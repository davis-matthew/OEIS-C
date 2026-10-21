/*
 * A227804  a(1) = greatest k such that H(k) - H(8) < H(8) - H(4); a(2) =
 *          greatest k such that H(k) - H(a(1)) < H(a(1)) - H(8), and for n >
 *          2, a(n) = greatest k such that H(k) - H(a(n-1)) > H(a(n-1)) -
 *          H(a(n-2)), where H = harmonic number.
 * https://oeis.org/A227804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 35, u64 n <= 74, u128 n <= 153,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A227804
   Conjectured recurrence:
   a(1) = 15, a(2) = 27, a(3) = 48, a(4) = 85
   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - a(n-4)   for n ≥ 5   */

#include <stddef.h>          /* for size_t, may be used by helpers */

#include "../utils/mul_small.h"   /* provides seq_t mul_small(seq_t, unsigned int) */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 1) return (seq_t)15;
    if (n == 2) return (seq_t)27;
    if (n == 3) return (seq_t)48;
    if (n == 4) return (seq_t)85;

    /* Use a sliding window of the last four values. */
    seq_t a1 = (seq_t)15;   /* a(n-4)  */
    seq_t a2 = (seq_t)27;   /* a(n-3)  */
    seq_t a3 = (seq_t)48;   /* a(n-2)  */
    seq_t a4 = (seq_t)85;   /* a(n-1)  */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t an = mul_small(a4, 3)          /* 3*a(n-1) */
                - mul_small(a3, 3)          /* -3*a(n-2) */
                + mul_small(a2, 2)          /* +2*a(n-3) */
                - a1;                      /* -a(n-4)  */

        /* Shift the window forward. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }

    return a4;
}
