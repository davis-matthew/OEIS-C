/*
 * A104677  a(n) = binomial(n+3,3)*binomial(n+8,3).
 * https://oeis.org/A104677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+3,3) * binomial(n+8,3)
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t c1 = ((x + (seq_t)3) * (x + (seq_t)2) * (x + (seq_t)1)) / ((seq_t)6);
    seq_t c2 = ((x + (seq_t)8) * (x + (seq_t)7) * (x + (seq_t)6)) / ((seq_t)6);
    return c1 * c2;
}
