/*
 * A036773  Number of labeled rooted trees with a degree constraint:
 *          ((5*n)!/(120^n)) * C(5*n+1, n).
 * https://oeis.org/A036773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((5*n)!/(120^n)) * C(5*n+1, n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

/* Computes a(n) for the OEIS sequence A036773.
 * Uses only arithmetic on seq_t and idx_t, no floating-point operations. */
seq_t a(idx_t n)
{
    idx_t m = 5 * n;            /* 5*n */
    seq_t f = (seq_t)1;         /* f will hold (5*n)! / 120^n  */

    for (idx_t k = 1; k <= m; ++k) {
        f *= (seq_t)k;           /* multiply by next factor of (5*n)!   */
        if (k % 5 == 0) {        /* every block of five numbers has a
                                    120 factor to cancel            */
            f /= (seq_t)120;      /* division keeps the value integer */
        }
    }

    seq_t b = binom(m + 1, n);   /* C(5*n+1, n) */
    return f * b;                /* final product gives a(n)          */
}
