/*
 * A034862  a(n) = n!*(3*n^2 - 15*n + 10)/6, n > 4.
 * https://oeis.org/A034862
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * (3*n^2 − 15*n + 10) / 6 for n > 4, * a(4) = 4
 *            (offset)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 4)           /* undefined below offset */
        return 0;
    if (n == 4)          /* offset value */
        return 4;

    /* Compute n! / 6 without overflow by cancelling the factors 2 and 3
       as soon as they appear.  For n ≥ 5, n! is guaranteed to contain
       at least one factor 2 and one factor 3.                       */
    seq_t fact_div6 = 1;
    bool need2 = true;
    bool need3 = true;

    for (idx_t i = 2; i <= n; ++i) {
        idx_t v = i;

        if (need2 && (v & 1u) == 0) {   /* v even, consume factor 2 */
            v >>= 1;
            need2 = false;
        }
        if (need3 && (v % 3u) == 0) {   /* v multiple of 3, consume factor 3 */
            v /= 3;
            need3 = false;
        }
        fact_div6 *= (seq_t)v;
    }

    /* At this point fact_div6 == n! / 6. */
    seq_t n_seq = (seq_t)n;
    seq_t coeff = 3 * n_seq * n_seq - 15 * n_seq + 10;
    return fact_div6 * coeff;
}
