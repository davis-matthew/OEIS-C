/*
 * A105334  a(n) = n*(n+1)/2 mod 32.
 * https://oeis.org/A105334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2 mod 32
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n modulo 64 */
    seq_t n_mod64 = ((seq_t)n) & 63;
    seq_t n1_mod64 = (((seq_t)n) + 1) & 63;

    /* Product modulo 64, then divide by 2 */
    seq_t prod = (n_mod64 * n1_mod64) & 63;
    return prod >> 1;
}
