/*
 * A103690  Triangle read by rows:
 *          T(n,k)=binomial(n,k-1)*k^(k-1)*(n+1-k)^(n-k) (1<=k<=n).
 * https://oeis.org/A103690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r, k-1) * k^(k-1) * (r+1-k)^(r-k) where r is the
 *            row number such that r*(r+1)/2 ≥ n, and k = n - r*(r-1)/2 is the
 *            column index
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 37 (2026-11-01)
 * Helpers:   binomial, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <sys/types.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/ipow_idx.h"

static inline void row_and_col(idx_t n, idx_t *row, idx_t *col)
{
    /* Find minimal r with r*(r+1)/2 ≥ n. */
    idx_t low = 0, high = 1;
    while (((__int128)high * (high + 1)) / 2 < n)
        high <<= 1;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (((__int128)mid * (mid + 1)) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    *row = low;
    __int128 prev_tri = ((__int128)*row * (*row - 1)) / 2;
    *col = (idx_t)(n - prev_tri);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined entry, return 0 */
    idx_t r, k;
    row_and_col(n, &r, &k);

    seq_t bin = binomial(r, k - 1);
    seq_t pow1 = ipow_idx((seq_t)k, k - 1);
    seq_t base2 = (seq_t)(r + 1 - k);
    seq_t pow2 = ipow_idx(base2, r - k);

    seq_t result = bin;
    result *= pow1;
    result *= pow2;
    return result;
}
