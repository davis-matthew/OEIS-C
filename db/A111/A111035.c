/*
 * A111035  Numbers k that divide the sum of the first k nonzero Fibonacci
 *          numbers.
 * https://oeis.org/A111035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 454, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A111035
   Numbers k that divide the sum of the first k nonzero Fibonacci numbers.
   The sum is F(k+2)-1, so a(n) is the successive k with k | (F(k+2)-1). */

/* The function below searches for the terms iteratively; the sequence is
   not given by a simple closed formula. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Fast doubling algorithm for Fibonacci numbers modulo a uint64_t modulus.
   This works for any k <= 2^64-1, which covers the k needed for n <= 10000. */
static idx_t fib_mod_uint64(idx_t n, idx_t mod)
{
    if (mod == 0) return 0;              /* undefined but never used */
    idx_t a = 0;                         /* F(0) */
    idx_t b = 1;                         /* F(1) */

    /* Process bits of n from high to low. */
    int shift = 63 - __builtin_clzll(n);  /* n > 0 for all calls */
    for (int i = shift; i >= 0; --i)
    {
        /* Doubling step: compute F(2k), F(2k+1) from F(k), F(k+1). */
        idx_t t1 = (idx_t)(((__uint128_t)a *
                            (((idx_t)2 * b % mod + mod - a) % mod)) % mod);
        idx_t t2 = (idx_t)(((__uint128_t)a * a + (__uint128_t)b * b) % mod);
        a = t1;
        b = t2;

        /* If the i‑th bit of n is 1, shift one step. */
        if ((n >> i) & 1)
        {
            idx_t t3 = (a + b) % mod;
            a = b;
            b = t3;
        }
    }
    return a;   /* F(n) mod mod */
}

/* Check whether k divides F(k+2)-1. */
static bool divides_k(idx_t k)
{
    idx_t mod = k;
    idx_t f_mod = fib_mod_uint64(k + 2, mod);  /* F(k+2) mod k */
    idx_t val = (f_mod + mod - 1) % mod;       /* (F(k+2)-1) mod k */
    return val == 0;
}

/* Public API: return the n-th term of sequence A111035 (n >= 1). */
seq_t a(idx_t n)
{
    /* Static table of already found terms. */
    static seq_t *list = NULL;   /* pointer to array of seq_t */
    static idx_t used = 0;       /* number of terms stored */
    static idx_t cap = 0;        /* allocated capacity */
    static idx_t next_k = 1;     /* next candidate k to test */

    /* Expand the list until it contains at least n terms. */
    while (used < n)
    {
        if (divides_k(next_k))
        {
            if (used == cap)
            {
                cap = (cap == 0) ? 16 : cap << 1;
                seq_t *tmp = (seq_t *)realloc(list,
                                               cap * sizeof(seq_t));
                if (!tmp) abort();           /* memory failure */
                list = tmp;
            }
            list[used++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return list[n - 1];
}
