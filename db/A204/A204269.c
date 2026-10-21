/*
 * A204269  Symmetric matrix: f(i,j)=floor[(i+j+2)/4]-floor[(i+j)/4], by
 *          (constant) antidiagonals.
 * https://oeis.org/A204269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A204269: Flattened matrix f(i,j)=⌊(i+j+2)/4⌋−⌊(i+j)/4⌋ along constant
 * antidiagonals starting from (1,1).  For each diagonal of length k−1
 * (k=i+j) we output the same value, namely 1 when k≡2,3 (mod 4) and 0 otherwise.
 *
 * We determine the diagonal length m that contains the nth term using
 * the triangular numbers: the first `m` diagonals contain exactly
 * m(m+1)/2 terms.  The smallest m with m(m+1)/2 ≥ n is found by
 * incremental trial (≤ ~150 iterations for n ≤ 10 000).  Then
 * a(n) = 1 if (m+1) mod 4 ≥ 2, otherwise 0.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* find the diagonal length m such that m*(m+1)/2 ≥ n */
    idx_t m = 1;
    unsigned __int128 tri = (unsigned __int128)m * (m + 1) / 2;
    while (tri < n)
    {
        ++m;
        tri = (unsigned __int128)m * (m + 1) / 2;
    }

    /* determine value in this diagonal: 1 for k≡2,3 (mod 4), else 0 */
    idx_t k = m + 1;           /* sum i+j for this diagonal */
    seq_t val = ((k % 4) >= 2) ? (seq_t)1 : (seq_t)0;
    return val;
}
