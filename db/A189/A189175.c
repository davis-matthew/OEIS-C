/*
 * A189175  Riordan matrix (1+x/sqrt(1-4*x),(1-sqrt(1-4*x))/2).
 * https://oeis.org/A189175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2, T(n,k) = binomial(2*n-k,
 *            n-k)*(n^2+n*k-k^2-k)/((2*n-k)*(2*n-k-1)). This is equivalent to
 *            the row‑by‑row enumeration of the Riordan matrix
 *            (1+x/√(1-4x),(1-√(1-4x))/2)
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

static idx_t triangular_row(idx_t n)
{
    /* Find the largest r with r*(r+1)/2 <= n  (r <= 2^33 for 64‑bit n). */
    idx_t low  = 0;
    idx_t high = (1ULL << 33);
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1ULL) / 2ULL;
        if (tri <= (unsigned __int128)n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine row r and position k in that row. */
    idx_t r = triangular_row(n);
    uint64_t row_start = (uint64_t)(((unsigned __int128)r * (r + 1ULL)) / 2ULL);
    idx_t k = n - row_start;

    /* Bell‑corner cases where the closed form would divide by zero. */
    if (r == 0 && k == 0) return (seq_t)1;
    if (r == 1 && k == 1) return (seq_t)1;

    /* Compute the binomial coefficient C(2r-k, r-k). */
    idx_t a_val = 2ULL * r - k;
    idx_t b_val = r - k;
    seq_t bin = binom(a_val, b_val);

    /* Numerator part: r^2 + r*k - k^2 - k. */
    seq_t sr = (seq_t)r;
    seq_t sk = (seq_t)k;
    seq_t num = sr * sr + sr * sk - sk * sk - sk;

    /* Denominator: (2r-k)*(2r-k-1). */
    seq_t tmp = (seq_t)(2ULL * r - k);
    seq_t den = tmp * (tmp - 1ULL);

    /* Return the exact integer value. */
    return bin * num / den;
}
