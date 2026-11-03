/*
 * A100992  Indices k of Fibonacci numbers F(k) (A000045) that are divisible
 *          by k+1.
 * https://oeis.org/A100992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that F(k) mod (k+1) == 0, where
 *            F(k) is the k-th Fibonacci number (F(0)=0, F(1)=1)
 * Range:     u8 n <= 25, u16 n <= 3266, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9816 (2026-11-03)
 * Helpers:   mem, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mulmod.h"
#include "../utils/mem.h"

/* fast‑doubling recursion: computes F(n) mod mod and F(n+1) mod mod.
   The pair is stored in *f and *f_next. */
static void fib_pair(idx_t n, uint64_t mod, uint64_t *f, uint64_t *f_next)
{
    if (n == 0) {
        *f = 0;
        *f_next = 1 % mod;   /* 1 but ensure modulo reduction */
        return;
    }
    uint64_t a, b;
    fib_pair(n >> 1, mod, &a, &b);          /* a = F(k),  b = F(k+1) */

    /* c = F(2k)   = a * (2*b - a) */
    uint64_t t = mulmod(b, 2, mod);    /* 2*b mod mod   */
    if (t < a) t += mod;                    /* ensure t >= a */
    t -= a;                                 /* (2*b - a) mod mod */
    uint64_t c = mulmod(a, t, mod);

    /* d = F(2k+1) = a*a + b*b */
    uint64_t d = (mulmod(a, a, mod) + mulmod(b, b, mod)) % mod;

    if ((n & 1) == 0) {                     /* n even */
        *f = c;
        *f_next = d;
    } else {                                /* n odd  */
        *f = d;
        *f_next = (c + d) % mod;
    }
}

/* Return F(n) modulo mod. */
static inline uint64_t fib_mod_uint64(idx_t n, idx_t mod)
{
    uint64_t f, f_next;
    fib_pair(n, (uint64_t)mod, &f, &f_next);
    return f;
}

/* Return the n-th element of OEIS A100992. */
seq_t a(idx_t n)
{
    static seq_t *table = NULL; /* stores found k values */
    static idx_t table_len = 0;
    static idx_t next_k = 1;    /* next k to test */

    while (table_len < n) {
        uint64_t mod = (uint64_t)(next_k + 1);
        if (fib_mod_uint64(next_k, mod) == 0) {
            seq_t *tmp = mem_resize(seq_t, table, (table_len + 1));
            if (!tmp) { /* allocation failure; abort */
                exit(EXIT_FAILURE);
            }
            table = tmp;
            table[table_len] = (seq_t)next_k;
            ++table_len;
        }
        ++next_k;
    }
    return table[n - 1];
}
