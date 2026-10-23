/*
 * A171650  Triangle T, read by rows : T(n,k) = A007318(n,k)*A026641(n-k).
 * https://oeis.org/A171650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{k=0..n} T(n,k) where T(n,k)=C(n,k)*A026641(n-k) with
 *            A026641(m) = abs( Σ_{j=0..m} (-1)^j * C(m+j,j) )
 *            A007318(n,k)=C(n,k). Read the triangle row‑wise (offset 0)
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 152, u64 n <= 498,
 *            u128 n <= 1776, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* binomial(idx_t a, idx_t b) -> seq_t */

/* Convert a linear index to the corresponding row r and column k.  */
static void index_to_row_col(idx_t n, idx_t *r, idx_t *k)
{
    /* r is the unique integer such that r*(r+1)/2 <= n < (r+1)*(r+2)/2  */
    idx_t low = 0, high = 1;
    while ((high * (high + 1)) / 2 <= n)
        high <<= 1;                     /* exponential search */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((mid * (mid + 1)) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    *r = low;
    *k = n - (low * (low + 1)) / 2;
}

/* Compute | Σ_{j=0..m} (-1)^j * C(m+j, j) |  */
static seq_t sum_abs(int m)
{
    seq_t pos = 0;   /* terms with even j (positive contributions) */
    seq_t neg = 0;   /* terms with odd  j (negative contributions) */

    for (int j = 0; j <= m; ++j) {
        seq_t term = binomial(m + j, j);
        if ((j & 1) == 0)
            pos += term;
        else
            neg += term;
    }
    return pos >= neg ? pos - neg : neg - pos;
}

/* External function required by the harness */
seq_t a(idx_t n)
{
    idx_t r, k;
    index_to_row_col(n, &r, &k);
    int m = (int)(r - k);            /* 0 <= m <= r <= sqrt(2*n) */

    seq_t bin_r_k = binomial(r, k);
    seq_t sum_abs_val = sum_abs(m);

    return bin_r_k * sum_abs_val;
}
