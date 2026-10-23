/*
 * A047179  Number of nonempty subsets of {1,2,...,n} in which exactly 4/5 of
 *          the elements are <= (n-1)/2.
 * https://oeis.org/A047179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = sum_{t>=1, 4t <= floor((n-1)/2), t <= n - floor((n-1)/2)}
 *                C( floor((n-1)/2), 4t ) * C( n - floor((n-1)/2), t )
 *
 * This counts nonempty subsets S ⊆ {1,…,n} with exactly 4/5 of their
 * elements ≤ floor((n-1)/2).  Let m = floor((n-1)/2).  Then we need
 * |S| = 5t (t ≥ 1), 4t elements from {1,…,m} and t elements from
 * {m+1,…,n}.  Each such choice contributes C(m,4t)*C(n-m,t).
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Compute m = floor((n-1)/2) and h = n - m */
    idx_t m = (n - 1) / 2;
    idx_t h = n - m;

    seq_t result = 0;

    /* t must satisfy 4t <= m and t <= h */
    idx_t max_t = m / 4;
    if (h < max_t) max_t = h;

    for (idx_t t = 1; t <= max_t; ++t)
    {
        idx_t low_k = 4 * t;
        seq_t c_low  = binomial(m, low_k);
        seq_t c_high = binomial(h, t);
        result += c_low * c_high;
    }

    return result;
}
