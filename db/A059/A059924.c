/*
 * A059924  Write the numbers from 1 to n^2 in a spiraling square; a(n) is the
 *          total of the sums of the two diagonals.
 * https://oeis.org/A059924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = (16*n^3 - 6*n^2 + 8*n + 3 - 3*(-1)^n) / 12
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 682, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t once to avoid repeated casting */

    seq_t sn = n;
    /* Compute 16*n^3 - 6*n^2 + 8*n */
    seq_t term = sn * sn * sn;          /* n^3      */
    term = (term * (seq_t)16);          /* 16*n^3   */
    seq_t n2 = sn * sn;                 /* n^2      */
    term = term - (n2 * (seq_t)6);     /* - 6*n^2  */
    term = term + (sn * (seq_t)8);      /* + 8*n    */

    /* Add correction depending on parity of n */
    /* 3 - 3*(-1)^n == 0 if n even, 6 if n odd */
    if ((n & (idx_t)1) != (idx_t)0) {   /* n odd */
        term = term + (seq_t)6;
    }

    /* Divide by 12 (exact division) */
    return term / (seq_t)12;
}
