/*
 * A146307  a(n) = denominator of (n-6)/(2n) = denominator of (n+6)/(2n).
 * https://oeis.org/A146307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, look
 * Formula:   not stated
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (2*n) / gcd(|n-6|, 2*n)
 * That is, a(n) is the denominator of the reduced fraction (n-6)/(2*n).
 */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t sn = (seq_t)n;

    /* Denominator: 2*n */
    seq_t den = sn * ((seq_t)2);

    /* Absolute value of (n-6) as seq_t */
    int64_t diff = (int64_t)n - 6;
    seq_t num_abs = (seq_t)(diff >= 0 ? diff : -diff);

    /* Compute gcd of |n-6| and 2*n */
    seq_t g = gcd(num_abs, den);

    /* Reduced denominator */
    return den / g;
}
