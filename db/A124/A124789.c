/*
 * A124789  Expansion of (1+x^2)/(1-x^4-x^5).
 * https://oeis.org/A124789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binom(floor(k/2), n-2*k)
 * Range:     u8 n <= 35, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t a_val = k >> 1;           /* floor(k/2) */
        idx_t b_val = n - (k << 1);     /* n-2*k */
        if (b_val > a_val) continue;    /* binom(a_val,b_val) is 0 in this case */
        sum += binom(a_val, b_val);
    }
    return sum;
}
