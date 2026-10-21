/*
 * A213936  Number triangle with entry a(n,k), n>=1, m=1, 2, ..., n, giving
 *          the number of representative necklaces with n beads (C_n symmetry)
 *          corresponding to the color multinomial c[1]^k*c[2]*...*c[n+1-k].
 * https://oeis.org/A213936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the nth term of the flattened triangle with entries
 *            a(r,k) = (r-1)!/k! for 1 ≤ k < r, and a(r,r)=1. The nth term is
 *            obtained by finding the row r such that (r-1)*r/2 < n ≤
 *            r*(r+1)/2, then k = n – (r-1)*r/2, and finally evaluating the
 *            above formula
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Find the row r such that cumulative terms up to r-1 < n ≤ cumulative terms up to r. */
    idx_t row = 1;
    idx_t terms = 1;  /* total terms up to the current row */
    while (terms < n) {
        ++row;
        terms += row;
    }

    /* Compute the column index k within row 'row'. */
    idx_t prev_terms = terms - row; /* terms before this row */
    idx_t k = n - prev_terms;       /* 1 ≤ k ≤ row */

    /* Handle the main diagonal entry. */
    if (k == row) {
        return (seq_t)1;
    }

    /* Compute (row-1)! / k! = product over i = k+1 .. row-1. */
    seq_t result = (seq_t)1;
    for (idx_t i = k + 1; i <= row - 1; ++i) {
        result *= (seq_t)i;
    }
    return result;
}
