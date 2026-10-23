/*
 * A190425  Number of one-sided prudent walks from (0,0) to (n,n), with
 *          floor(n/2)+n east steps, floor(n/2) west steps and n north steps.
 * https://oeis.org/A190425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   For n = 0 : a(0) = 1 For n > 0: let k = n / 2 (integer division)
 *            if n is even: a(n) = binomial(2n, n) * binomial(3k - 1, k) if n
 *            is odd : a(n) = binomial(2n, n) * binomial(3k, k) This matches
 *            the OEIS values for all n ≥ 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 40,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t k = n / 2;
    idx_t m_top = (n & 1) ? (3 * k) : (3 * k - 1);   /* 3*k for odd, 3*k-1 for even */

    seq_t first  = binomial(2 * n, n);
    seq_t second = binomial(m_top, k);

    return first * second;
}
