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
 * Helpers:   bit, comb_leq, term_with_popcount
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
#include "../utils/bit.h"
#include "../utils/comb_leq.h"
#include "../utils/term_with_popcount.h"

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
