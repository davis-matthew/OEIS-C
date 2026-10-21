/*
 * A072898  Numerator of c(n) where c(0)=1, c(n+1) = n/c(n) + 1.
 * https://oeis.org/A072898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = numerator of c(n) where * c(0) = 1, * c(k+1) = k / c(k) +
 *            1, * and c(k) is reduced to lowest terms at each step
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 21, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   gcd_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_seq_t.h"

seq_t a(idx_t n)
{
    seq_t p = (seq_t)1;   /* numerator */
    seq_t q = (seq_t)1;   /* denominator */

    for (idx_t i = (idx_t)0; i < n; ++i) {
        seq_t new_p = (seq_t)i * q + p;  /* numerator for next term */
        seq_t new_q = p;                 /* denominator for next term */
        seq_t g = gcd_seq_t(new_p, new_q); /* reduce fraction */
        p = new_p / g;
        q = new_q / g;
    }
    return p;  /* numerator of c(n) */
}
