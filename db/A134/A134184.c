/*
 * A134184  A transform of the central binomial coefficients A001405.
 * https://oeis.org/A134184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n-k,k) *
 *            binomial(n-k,floor((n-k)/2)) (OEIS A134184)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the helper for binomial coefficients */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 2;                     /* floor(n/2) */
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t nmk = n - k;                   /* n - k */
        seq_t t1 = binom(nmk, k);            /* C(n-k, k) */
        seq_t t2 = binom(nmk, nmk / 2);      /* C(n-k, floor((n-k)/2)) */
        sum += t1 * t2;
    }
    return sum;
}
