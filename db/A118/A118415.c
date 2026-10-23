/*
 * A118415  a(n) = (4*n - 3) * 2^(n - 1).
 * https://oeis.org/A118415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n - 3) * 2^(n - 1)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2^(n-1) by repeated left shift */
    seq_t pow = 1;
    for (idx_t i = 1; i < n; ++i)
        pow <<= 1;  /* safe for unsigned types */

    /* Compute the coefficient (4*n - 3) */
    seq_t coeff = (seq_t)4 * n - 3;

    return pow * coeff;
}
