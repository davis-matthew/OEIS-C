/*
 * A232599  Alternating sum of cubes, i.e., Sum_{k=0..n} k^p*q^k for p=3,
 *          q=-1.
 * https://oeis.org/A232599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ((-1)^n*(4*n^3+6*n^2-1)+1)/8
 * Range:     i8 n <= 5, i16 n <= 39, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Use __int128 for intermediate arithmetic to avoid overflow
       on small seq_t types while still producing correct results
       for large n.  The final result fits in seq_t because it
       is part of the sequence. */
    __int128 i = n;                          /* cast index to 128-bit */
    __int128 inner = 4*i*i*i + 6*i*i - 1;     /* 4*n^3 + 6*n^2 - 1 */
    if (n & 1) inner = -inner;               /* multiply by (-1)^n */
    __int128 res = (inner + 1) / 8;           /* divide by 8, exact division */
    return (seq_t)res;
}
