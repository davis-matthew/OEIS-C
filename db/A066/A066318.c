/*
 * A066318  Number of necklaces with n labeled beads of 2 colors.
 * https://oeis.org/A066318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * 2^n (n ≥ 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Calculate (n-1)! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i < n; ++i)
        fact *= (seq_t)i;

    /* Calculate 2^n by repeated multiplication to avoid
       undefined behaviour for large shift counts. */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= (seq_t)2;

    return fact * pow2;
}
