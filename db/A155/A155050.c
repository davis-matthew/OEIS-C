/*
 * A155050  A symmetric Catalan based triangle.
 * https://oeis.org/A155050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = Catalan( min(j, i-j) ) where n = i(i+1)/2 + j and 0 ≤ j ≤
 *            i
 * Range:     u8 n <= 83, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The Catalan helper is provided by the build. */

#include "../utils/catalan.h"

/* Helper: returns the i-th triangular number i*(i+1)/2 */
static inline idx_t triangular(idx_t i)
{
    /* use 128-bit to avoid overflow for intermediate multiplication */
    return (idx_t)(((__int128)i * (i + 1)) / 2);
}

/* Helper: given n, finds the largest i such that triangular(i) ≤ n */
static inline idx_t row_of_n(idx_t n)
{
    idx_t low = 0, high = 1;

    /* Expand high until triangular(high) > n */
    while (triangular(high) <= n) {
        high <<= 1ULL;
        /* guard against overflow of high (high becomes 0 after wrap) */
        if (high == 0) {
            high = (idx_t)-1;
            break;
        }
    }

    /* Binary search for exact row */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if (triangular(mid) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Find row i and column j for index n */
    idx_t i = row_of_n(n);
    idx_t j = n - triangular(i);

    /* Compute min(j, i-j) */
    idx_t m = (j > i - j) ? (i - j) : j;

    /* Return the Catalan number C_m */
    return catalan(m);
}
