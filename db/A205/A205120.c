/*
 * A205120  Ordered differences of distinct numbers k*(2^(k-1)).
 * https://oeis.org/A205120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = k*2^(k-1) - j*2^(j-1) where k is the least integer such
 *            that k*(k-1)/2 >= n, j = n - (k-1)*(k-2)/2. This enumerates all
 *            positive differences s(k)-s(j) for k>j, with s(k)=k*2^(k-1), in
 *            the order (k=2,j=1),
 *            (k=3,j=1),(k=3,j=2),(k=4,j=1),(k=4,j=2),(k=4,j=3),
 * Range:     u8 n <= 15, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"      /* seq_t pow2(idx_t exp); */
#include "../utils/mul_small.h" /* seq_t mul_small(seq_t x, unsigned int m); */

seq_t a(idx_t n)
{
    if (n == 0) /* offset 1: a(1) is first term; behaviour for n<1 is irrelevant */
        return (seq_t)0;

    /* Find k such that k*(k-1)/2 >= n, with k>=2. */
    idx_t k = 2;
    /* Loop small: k is O(sqrt(n)). For n up to 10000 k <= 142. */
    while ((k * (k - 1)) / 2 < n)
        ++k;

    /* preceding terms before block k: (k-1)*(k-2)/2 */
    idx_t preceding = ((k - 1) * (k - 2)) / 2;
    idx_t j = n - preceding;          /* 1 <= j <= k-1 */

    seq_t pow2_k = pow2(k - 1);
    seq_t pow2_j = pow2(j - 1);

    seq_t s_k = mul_small(pow2_k, (unsigned int)k);
    seq_t s_j = mul_small(pow2_j, (unsigned int)j);

    return s_k - s_j;
}
