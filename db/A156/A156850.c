/*
 * A156850  a(1)=2, a(n+1) is the smallest prime > (a(n) + sum of digits of
 *          a(n)).
 * https://oeis.org/A156850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   digit_sum, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2, a(n+1)= smallest prime > (a(n)+digit_sum(a(n))) */

#include <stddef.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                    /* undefined for n<1 */

    seq_t val = (seq_t)2;                          /* a(1) */
    if (n == 1) return val;

    /* iterate from a(2) to a(n) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t sum = val + digit_sum(val);
        seq_t cand = sum + (seq_t)1;               /* start searching after sum */
        while (!is_prime_big(cand)) ++cand;        /* find next prime */
        val = cand;                                /* update current term */
    }
    return val;
}
