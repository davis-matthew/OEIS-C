/*
 * A200432  Number of -n..n arrays x(0..4) of 5 elements with zero sum and no
 *          two or three adjacent elements summing to zero.
 * https://oeis.org/A200432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (115*n^4 - 130*n^3 + 65*n^2 - 50*n) / 12 (equivalent
 *            polynomial with rational coefficients) a(1) = 0, a(2) = 80, a(3)
 *            = 520,
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 80, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t */
    seq_t nt = (seq_t)n;
    /* Compute powers of n */
    seq_t n2 = nt * nt;
    seq_t n3 = n2 * nt;
    seq_t n4 = n3 * nt;
    /* Numerator of the polynomial: 115*n^4 - 130*n^3 + 65*n^2 - 50*n */
    seq_t numer = (seq_t)115 * n4
                - (seq_t)130 * n3
                + (seq_t)65  * n2
                - (seq_t)50  * nt;
    /* Divide by 12 to get the integer result */
    return numer / (seq_t)12;
}
