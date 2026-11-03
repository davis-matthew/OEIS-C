/*
 * A067576  Array T(i,j) read by downward antidiagonals, where T(i,j) is the
 *          j-th term whose binary expansion has i 1's.
 * https://oeis.org/A067576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 136, u32 n <= 528, u64 n <= 2080,
 *            u128 n <= 8001, big any n
 * Validated: standard, pass, 82 terms through n = 7697 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A067576
 *  a(n) is obtained by reading the array T(i,j) of numbers having i
 *  1‑bits in their binary expansion, j indexed among those with the
 *  same i, along downward antidiagonals.
 *
 *  We compute the (i,j) pair from n using antidiagonal counting, then
 *  find the j‑th integer with exactly i 1‑bits by the combinatorial
 *  number system representation.
 */

#include <stdint.h>   /* for uint*_t types (seq_t, idx_t are defined by the build) */
#include "../utils/pow2.h"

static idx_t comb_leq(idx_t n, idx_t k, idx_t limit)
/*  Return nCk but stop and return limit+1 as soon as the value exceeds
 *  limit.  If k>n, return 0.
 */
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    unsigned __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (unsigned __int128)(n - k + i) / (unsigned __int128)i;
        if (res > (unsigned __int128)limit) return limit + 1;
    }
    return (idx_t)res;
}

static seq_t term_with_popcount(idx_t popcnt, idx_t rank)
/*  Return the rank‑th (1‑based) positive integer with exactly popcnt
 *  bits equal to 1.  The algorithm uses the combinatorial number system.
 */
{
    idx_t j0 = rank - 1;          /* 0‑based rank */
    idx_t rem = popcnt;           /* remaining bits to place */

    /* Find the highest bit position that might be used. */
    idx_t pos;
    for (pos = popcnt - 1; ; ++pos) {
        if (comb_leq(pos + 1, rem, j0) > j0) break;
    }

    seq_t result = 0;
    for (idx_t p = pos; p != (idx_t)(-1); --p) { /* p unsigned, so stop at -1 */
        if (rem == 0) break;
        idx_t cnt = comb_leq(p, rem, j0);
        if (cnt <= j0) {
            result |= pow2(p);
            j0 -= cnt;
            --rem;
        }
    }
    return result;
}

static idx_t find_diagonal(idx_t n)
/*  Return the diagonal number d (≥2) that contains the nth element.
 *  The total number of elements up to diagonal d is d*(d-1)/2.
 */
{
    if (n <= 1) return 2;
    idx_t low = 2, high = 4;
    while (high * (high - 1) / 2 < n) high <<= 1;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid * (mid - 1) / 2 >= n) high = mid;
        else low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
/*  OEIS A067576: a(1)=1, a(2)=2, ... */
{
    if (n == 0) return 0;          /* value for n=0 is undefined; harmless */

    /* Locate the diagonal and the position within it. */
    idx_t d = find_diagonal(n);                     /* diagonal number (≥2) */
    idx_t cum_prev = (d - 2) * (d - 1) / 2;          /* elements before d */
    idx_t k = n - cum_prev - 1;                      /* 0‑based index within diagonal */

    /* Map to (i,j) coordinates.  i = k+1, j = d - i. */
    idx_t i = k + 1;                 /* number of 1‑bits */
    idx_t j = d - i;                 /* rank among numbers with i 1‑bits */

    return term_with_popcount(i, j);
}
