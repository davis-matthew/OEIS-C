/*
 * A159277  Ways to write the identity as a product of n 3-cycles in symmetric
 *          group S_4.
 * https://oeis.org/A159277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1; for n>=1, a(n) = (2/3)*(-1)^(n-1)*(((-8)^(n-1)) -
 *            (4^(n-1)))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0)            /* exact value for the offset 0 */
        return (seq_t)1;

    idx_t k = n - 1;                         /* use k = n-1 for the exponent */

    seq_t pow4 = pow_int((seq_t)4, k);       /* 4^k */
    seq_t pow8 = pow_int((seq_t)8, k);       /* 8^k */

    seq_t abs_diff;
    if (k & 1ULL) {                          /* k is odd  →  (-8)^k = -8^k  */
        abs_diff = pow8 + pow4;              /* | (-8)^k - 4^k | = 8^k + 4^k */
    } else {                                 /* k is even →  (-8)^k = 8^k   */
        abs_diff = pow8 - pow4;              /* | (-8)^k - 4^k | = 8^k - 4^k */
    }

    seq_t res = abs_diff * 2;                /* multiply by 2 */
    res = res / 3;                           /* divide by 3 – always exact */

    return res;
}
