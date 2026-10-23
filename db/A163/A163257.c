/*
 * A163257  An interspersion: the order array of the even-numbered columns
 *          (after swapping the first two rows) of the double interspersion at
 *          A161179.
 * https://oeis.org/A163257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = S(i,j) where (i,j) is chosen by antidiagonal scanning of
 *            the array S given by S(1,k) = k^2 + k - 1 S(2,k) = k^2 + k if
 *            n>1 odd: S(n,k) = k^2 + (n-1)k + (n-1)(n-3)/4 if n>2 even:
 *            S(n,k) = k^2 + (n-1)k + n(n-4)/4 The pair (i,j) for a given
 *            index n is found by let s = smallest integer with s*...
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
 
seq_t a(idx_t n)
{
    /* Find diagonal number s such that s*(s-1)/2 >= n */
    idx_t s = 1;
    while (((__int128)s * (s - 1)) / 2 < n) ++s;

    /* Number of elements before this diagonal */
    idx_t prev_total = ((__int128)(s - 1) * (s - 2)) / 2;

    /* Position within diagonal (0‑based) */
    idx_t offset = n - prev_total - 1;

    /* Convert to table indices */
    idx_t i = offset + 1;
    idx_t j = s - 1 - offset;

    /* Compute S(i,j) */
    seq_t val;
    if (i == 1) {
        val = (seq_t)j * j + j - 1;
    } else if (i == 2) {
        val = (seq_t)j * j + j;
    } else if (i & 1) {          /* odd i > 1 */
        val = (seq_t)j * j
              + (seq_t)(i - 1) * j
              + ((seq_t)(i - 1) * (i - 3)) / 4;
    } else {                     /* even i > 2 */
        val = (seq_t)j * j
              + (seq_t)(i - 1) * j
              + ((seq_t)i * (i - 4)) / 4;
    }

    return val;
}
