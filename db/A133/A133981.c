/*
 * A133981  Triangle read by rows: A000012 * A127701 + A127701 * A000012 -
 *          A000012 as infinite lower triangular matrices.
 * https://oeis.org/A133981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r + c + 1 if c < r; a(n) = 2*r - 1 if c = r, where r is
 *            the smallest integer with r(r+1)/2 ≥ n, and c = n - r(r-1)/2
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/triangular.h"

static idx_t find_row(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (triangular(mid) < (seq_t)n)
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    seq_t t_before = triangular(r - 1);
    idx_t c = n - (idx_t)t_before;
    if (c == r)
        return ((seq_t)r) * 2 - 1;
    else
        return ((seq_t)r) + (seq_t)c + 1;
}
