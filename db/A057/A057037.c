/*
 * A057037  Let R(i,j) be the rectangle with antidiagonals 1; 2,3; 4,5,6; ...;
 *          each k is an R(i(k),j(k)) and A057037(n)=j(2n-1).
 * https://oeis.org/A057037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = j(2n-1) where j(i,j) is the column coordinate of the
 *            entry numbered i in the rectangle whose antidiagonals are 1;
 *            2,3; 4,5,6; …, i.e. index(i,j) = (i+j-2)(i+j-1)/2 + i
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t diag_of_idx(idx_t n)
{
    /* Find least t such that t*(t+1)/2 >= n  (n>=1). */
    uint64_t low = 0, high = 1;
    /* grow high until it satisfies the condition */
    while ((unsigned __int128)high * (high + 1) / 2 < (unsigned __int128)n)
        high <<= 1;
    /* binary search */
    while (low + 1 < high) {
        uint64_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= (unsigned __int128)n)
            high = mid;
        else
            low = mid;
    }
    return high;
}

static inline seq_t j_from_index(idx_t N)
{
    uint64_t t = diag_of_idx(N);                 /* diagonal number */
    unsigned __int128 prev = (unsigned __int128)(t - 1) * t / 2; /* tri(t-1) */
    uint64_t offset = N - (idx_t)prev;           /* 1-based offset in diag */
    uint64_t j = t + 1 - offset;                 /* column index */
    return (seq_t)j;
}

seq_t a(idx_t n)
{
    /* compute a(n) = j(2n-1) */
    unsigned __int128 N128 = ((unsigned __int128)n << 1) - 1; /* 2n-1 */
    idx_t N = (idx_t)N128;                                   /* fits in idx_t */
    return j_from_index(N);
}
