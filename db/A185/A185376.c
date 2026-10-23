/*
 * A185376  Number of binary necklaces of 2n beads for which a cut exists
 *          producing a palindrome.
 * https://oeis.org/A185376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) + 2^((n-2^t)/(2^(t+1))) where t is the exponent
 *            of 2 in n
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow_int.h"

/* Count the number of times 2 divides n (v2) */
static inline idx_t v2(idx_t n)
{
    idx_t cnt = 0;
    while ((n & 1ull) == 0)
    {
        cnt++;
        n >>= 1;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* The exponent of 2 in n */
    idx_t t = v2(n);

    /* First term: 2^(n-1) */
    seq_t term1 = pow_int((seq_t)2, n - 1);

    /* Compute odd part of n after removing 2^t */
    idx_t odd = n >> t;            /* odd part, m in the description */

    /* Second term: 2^((m-1)/2) where m is odd part */
    idx_t exp2 = (odd - 1) / 2;    /* integer division, guaranteed exact */

    seq_t term2 = pow_int((seq_t)2, exp2);

    return term1 + term2;
}
