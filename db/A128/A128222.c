/*
 * A128222  A127701 * A128174.
 * https://oeis.org/A128222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   For n ≥ 1, let r be the smallest integer with r*(r+1)/2 ≥ n. Let
 *            j = n - r*(r-1)/2 - 1 be the 0‑based index within row r. a(n) =
 *            r if r and j have different parity, otherwise a(n) = 1
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: find the row index r for a given linear index n. */
static idx_t
row_of(idx_t n)
{
    /* r ≤ 2^32−1 for n ≤ 2^64−1, so this bound is safe. */
    idx_t low = 1, high = 0xFFFFFFFFULL;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low  = mid + 1;
    }
    return low;
}

seq_t
a(idx_t n)
{
    idx_t r = row_of(n);
    /* Triangular number T(r-1) = r*(r-1)/2 */
    unsigned __int128 tri_prev = (unsigned __int128)r * (r - 1) / 2;
    idx_t start = (idx_t)tri_prev + 1;          /* first index in row r */
    idx_t j = n - start;                       /* column index (0‑based) */
    /* a(n) = r if r and j have opposite parity, else 1 */
    if ((r & 1ULL) != (j & 1ULL))
        return (seq_t)r;
    else
        return (seq_t)1;
}
