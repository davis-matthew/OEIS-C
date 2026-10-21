/*
 * A070216  Triangle T(n, k) = n^2 + k^2, 1 <= k <= n, read by rows.
 * https://oeis.org/A070216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A070216 (OEIS) – Triangle T(n,k) = n^2 + k^2, read by rows.
   Computation:  given index n (n≥1), find the row r such that
   r*(r-1)/2 < n ≤ r*(r+1)/2.  Let k = n - r*(r-1)/2 (1≤k≤r).
   Then a(n) = r^2 + k^2.  All arithmetic is performed in seq_t. */

#include <stdint.h>

static idx_t
row_of(idx_t n)            /* smallest r with r*(r+1)/2 >= n  */
{
    idx_t low = 1, high = 1;
    /* grow high until it's large enough */
    while ((((__int128)high * (high + 1)) >> 1) < n)
        high <<= 1;
    /* binary search */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((((__int128)mid * (mid + 1)) >> 1) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t
a(idx_t n)
{
    /* Find row r and column k */
    idx_t r = row_of(n);
    /* previous elements before this row */
    idx_t prev = ((r - 1) * r) >> 1;
    idx_t k = n - prev;          /* 1 ≤ k ≤ r */

    /* Compute the value in seq_t */
    seq_t sr = (seq_t)r;
    seq_t sk = (seq_t)k;
    return sr * sr + sk * sk;
}
