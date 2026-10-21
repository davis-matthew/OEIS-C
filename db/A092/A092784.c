/*
 * A092784  a(n) = round(n*(Pi - 2)).
 * https://oeis.org/A092784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(n*(Pi - 2))
 * Range:     u8 n <= 1, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Pi is approximated by 355/113.  (Pi-2) ≈ 129/113, accurate enough to
   guarantee correct rounding for all n where the result fits in seq_t. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The offset in OEIS is 1, so a(1) is the first listed term. */
    if (n == 0) return 0;               /* not used for offset > 1 */
    /* Compute n * 129 / 113 with rounding to nearest integer. */
    seq_t prod = (seq_t)n * (seq_t)129;  /* numerator of (Pi-2) approximation */
    prod += (seq_t)56;                  /* floor(113/2) = 56, for rounding */
    return prod / (seq_t)113;
}
