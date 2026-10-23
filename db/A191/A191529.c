/*
 * A191529  Number of dispersed Dyck paths of length n (i.e., Motzkin paths of
 *          length n with no (1,0) steps at positive heights) with no initial
 *          and no final (1,0)-steps.
 * https://oeis.org/A191529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 0. For even n (n = 2*k, k ≥ 1): a(n) = C(2*k-2,
 *            k-1). For odd n (n = 2*k+1, k ≥ 2): a(n) = C(2*k-1, k-2). For
 *            odd n with k < 2 (i.e., n = 1 or 3) the value is 0
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    if ((n & 1) == 0) {
        /* n is even: n = 2*k, k ≥ 1   */
        idx_t k = n >> 1;                 /* k = n/2   */
        return binomial((2 * k) - 2, k - 1);
    } else {
        /* n is odd: n = 2*k+1, k ≥ 0   */
        idx_t k = (n - 1) >> 1;           /* k = (n-1)/2   */
        if (k < 2) return (seq_t)0;      /* n = 1 or 3 */
        return binomial((2 * k) - 1, k - 2);
    }
}
