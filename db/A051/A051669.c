/*
 * A051669  a(n) = 11*2^n - 4*n - 10.
 * https://oeis.org/A051669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 11*2^n - 4*n - 10
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2^n in seq_t. */
    seq_t pow2 = (seq_t)1;
    seq_t two  = (seq_t)2;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= two;

    /* a(n) = 11*2^n - 4*n - 10 */
    seq_t term = (seq_t)11 * pow2 - (seq_t)4 * (seq_t)n - (seq_t)10;
    return term;
}
