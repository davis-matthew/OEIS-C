/*
 * A124216  Generalized Pascal triangle.
 * https://oeis.org/A124216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) where T(r,k) = Σ_{j=k..r}
 *            C(r,j)⋅C(j,2(j−k))⋅2^(j−k). The indices r and k are the unique
 *            pair satisfying r·(r+1)/2 ≤ n < (r+1)·(r+2)/2
 * Range:     u8 n <= 23, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
#if defined(__SIZEOF_INT128__)
        /* When seq_t fits in 64 bits, use 128‑bit intermediate to avoid overflow of the product. */
        if (sizeof(seq_t) <= sizeof(uint64_t)) {
            __uint128_t temp = (__uint128_t)res * (n - k + i);
            res = (seq_t)(temp / i);
        } else
#endif
        {
            res = res * (seq_t)(n - k + i) / (seq_t)i;
        }
    }
    return res;
}

static seq_t pow2_exp(idx_t exp)
{
    seq_t val = 1;
    for (idx_t i = 0; i < exp; ++i)
        val *= 2;          /* multiplication is safe because any overflow is only accepted
                               when the result exceeds the width of seq_t. */
    return val;
}

seq_t a(idx_t n)
{
    /* Find the row r and position k for the flattened index n. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t start_of_row = r * (r + 1) / 2;   /* first index of row r */
    idx_t k = n - start_of_row;            /* column index within the row */

    seq_t sum = 0;
    for (idx_t j = k; j <= r; ++j) {
        idx_t m = (j - k) << 1;            /* 2*(j−k) */
        if (m > j) continue;              /* C(j,m) would be zero, skip */

        seq_t term = binom(r, j);
        term = term * binom(j, m);
        term = term * pow2_exp(j - k);
        sum += term;
    }
    return sum;
}
