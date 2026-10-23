/*
 * A173476  Triangle T(n, k) = 1 + (k!)^2 - 2*k!*(n-k)! + ((n-k)!)^2, read by
 *          rows.
 * https://oeis.org/A173476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 + ((n-k)! - k!)^2 where (r,k) are indices of the * n‑th
 *            term in the triangle T(n,k) read row‑wise with offset 0. * Row r
 *            satisfies r*(r+1)/2 <= n < (r+1)*(r+2)/2, * and k = n -
 *            r*(r+1)/2. T(r,k) = 1 + ((r-k)! - k!)^2
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 44, u64 n <= 90, u128 n <= 230,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factorial.h"

static inline idx_t triangle_row(idx_t n)
{
    idx_t r = 0;
    while ((unsigned __int128)r * (r + 1) / 2 <= n)
        ++r;
    return r - 1;
}

seq_t a(idx_t n)
{
    idx_t r = triangle_row(n);
    idx_t offset = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - offset;                /* position within the row */

    seq_t fact_k  = factorial(k);
    seq_t fact_nk = factorial(r - k);

    seq_t diff = (fact_k > fact_nk) ? (fact_k - fact_nk) : (fact_nk - fact_k);
    return diff * diff + (seq_t)1;
}
