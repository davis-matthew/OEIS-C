/*
 * A138986  a(n) = Frobenius number for 6 successive numbers = F(n+1, n+2,
 *          n+3, n+4, n+5, n+6).
 * https://oeis.org/A138986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + (n+1)*floor((n-1)/5)
 * Range:     u8 n <= 35, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index once; subsequent arithmetic uses seq_t */
    seq_t n_t = (seq_t)n;
    seq_t term_mult = (seq_t)(n + 1);                 /* (n+1) */
    seq_t floor_val = ((seq_t)(n - 1)) / 5;           /* floor((n-1)/5) */
    return n_t + term_mult * floor_val;
}
