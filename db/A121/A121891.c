/*
 * A121891  Least m such that (F(n) mod m) > (F(n+2) mod m).
 * https://oeis.org/A121891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m such that (FibStd(n+1) mod m) > (FibStd(n+3) mod
 *            m), where FibStd is the standard Fibonacci sequence with
 *            FibStd(0)=0, FibStd(1)=1
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Validated: partial, pass, 101 terms through n = 101 (2026-11-01)
 * Helpers:   mulmod_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code uses fast‑doubling with safe modular multiplication. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mulmod_seq.h"

/* Fast‑doubling algorithm that computes FibStd(n) and FibStd(n+1) modulo mod.
   The routine assumes n >= 0 and mod >= 2.  The results are stored in *fn
   (FibStd(n) mod mod) and *fn1 (FibStd(n+1) mod mod). */
static inline void fib_std_mod(idx_t n, seq_t mod, seq_t *fn, seq_t *fn1)
{
    seq_t a = 0;          /* FibStd(0) mod mod */
    seq_t b = 1;          /* FibStd(1) mod mod */

    /* Find the most significant bit of n. */
    idx_t msb = (idx_t)1 << (sizeof(idx_t) * 8 - 1);
    while ((msb & n) == 0) msb >>= 1;

    for (; msb; msb >>= 1) {
        /* Doubling step: compute FibStd(2k) and FibStd(2k+1). */
        seq_t two_b = (b << 1) % mod;            /* 2*b mod */
        seq_t t = (two_b + mod - a) % mod;       /* (2*b - a) mod */
        seq_t c = mulmod_seq(a, t, mod);    /* FibStd(2k) mod */
        seq_t d = (mulmod_seq(a, a, mod) + mulmod_seq(b, b, mod)) % mod;
        /* d = FibStd(2k+1) mod */

        if (n & msb) { /* odd step: advance to k+1 */
            a = d;
            b = (c + d) % mod;                   /* FibStd(2k+2) mod */
        } else {              /* even step */
            a = c;
            b = d;
        }
    }
    *fn  = a;      /* FibStd(n) mod mod */
    *fn1 = b;      /* FibStd(n+1) mod mod */
}

seq_t a(idx_t n)
{
    /* Shift n by 1 so that we work with FibStd(n+1) and FibStd(n+3). */
    idx_t k = n + 1;                 /* k = n+1 */
    for (seq_t m = 2; ; ++m) {
        seq_t fn, fn1;
        fib_std_mod((idx_t)k, m, &fn, &fn1);   /* fn  = FibStd(k)   = FibStd(n+1)
                                                  fn1 = FibStd(k+1) = FibStd(n+2) */

        /* FibStd(n+3) = FibStd(n+1) + FibStd(n+2) mod m */
        seq_t fn3 = (fn + fn1) % m;

        if (fn > fn3) {
            return m;
        }
    }
}
