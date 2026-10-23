/*
 * A119831  Number triangle (3n)!/(3k)!.
 * https://oeis.org/A119831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = product_{i=3k+1}^{3r} i, where r is the unique integer
 *            with r(r+1)/2 ≤ n < (r+1)(r+2)/2 and k = n - r(r+1)/2. This is
 *            the (n‑th) element of the triangle T(m,k) = (3m)!/(3k)! read
 *            row‑wise
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 14, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* triangular number: t(r) = r*(r+1)/2, computed in 128‑bit to avoid overflow */
static inline idx_t triangular(idx_t r)
{
    return (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
}

/* Find r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 using binary search. */
static idx_t find_row(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;
    /* grow high until triangular(high) > n or overflow */
    while (triangular(high) <= n)
    {
        if (high > (UINT64_MAX >> 1))
            break;              /* avoid infinite loop if n is huge */
        high <<= 1;
    }
    /* binary search in [low, high) */
    while (high - low > 1)
    {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    /* Determine row and column indices. */
    idx_t r = find_row(n);
    idx_t k = n - triangular(r);

    /* Compute product from 3k+1 to 3r inclusive. */
    seq_t result = (seq_t)1;
    idx_t start = 3 * k + 1;
    idx_t end   = 3 * r;
    for (idx_t i = start; i <= end; ++i)
    {
        result *= (seq_t)i;
    }
    return result;
}
