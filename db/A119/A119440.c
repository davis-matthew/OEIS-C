/*
 * A119440  Triangle read by rows: T(n,k) is the number of binary sequences of
 *          length n that start with exactly k 01's (0 <= k <= floor(n/2)).
 * https://oeis.org/A119440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 80, u32 n <= 288, u64 n <= 1088,
 *            u128 n <= 2549, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A119440: a(n) is the nth term obtained by reading
   the triangle T(m,k) by rows.  For 0 ≤ k ≤ ⌊m/2⌋,
   T(m,k) = 1          if m = 2k,
            2          if m = 2k+1,
            3·2^(m-2k-2) otherwise.
   For a given n we find the row r with
     cumulative_terms(r-1) ≤ n < cumulative_terms(r),
   then k = n - cumulative_terms(r-1) and a(n) = T(r,k).   */

#include <stdint.h>
#include "../utils/pow2.h"

/* cumulative number of triangle entries up to and including row r */
static inline unsigned __int128 cumulative_terms(idx_t r)
{
    idx_t floor_half = r >> 1;            /* ⌊r/2⌋   */
    idx_t ceil_half  = (r + 1) >> 1;      /* ⌈r/2⌉   */
    return (unsigned __int128)(r + 1) +
           (unsigned __int128)floor_half * (unsigned __int128)ceil_half;
}

/* compute a(n) as defined above */
seq_t a(idx_t n)
{
    idx_t r = 0;
    unsigned __int128 n128 = n;

    /* find smallest r such that cumulative_terms(r) > n */
    while (cumulative_terms(r) <= n128) {
        ++r;
    }

    /* cumulative terms of rows before r */
    unsigned __int128 prevt128 = (r == 0) ? (unsigned __int128)0
                                          : cumulative_terms(r - 1);
    idx_t k = (idx_t)(n128 - prevt128);  /* column index within the row */

    /* evaluate T(r,k) according to the definition */
    if (r == 2 * k) {
        return (seq_t)1;
    }
    if (r == 2 * k + 1) {
        return (seq_t)2;
    }
    /* otherwise r ≥ 2k+2 */
    idx_t exponent = r - 2 * k - 2;
    return (seq_t)3 * pow2(exponent);
}
