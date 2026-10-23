/*
 * A179431  a(n) = binomial(3^(n-1), n).
 * https://oeis.org/A179431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(3^(n-1), n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

/* Compute binomial coefficient C(3^(n-1), n) for n >= 0.
   seq_t is an arbitrary‑precision integer type or an unsigned integer.   */
seq_t a(idx_t n)
{
    if (n == 0)                     /* a(0) = 1 */
        return 1;

    seq_t N   = pow3(n - 1);        /* N = 3^(n-1) */
    seq_t res = 1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t factor = N - (seq_t)(i - 1);   /* N - (i-1) */
        res = (res * factor) / (seq_t)i;    /* divide by i */
    }
    return res;
}
