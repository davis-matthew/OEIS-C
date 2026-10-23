/*
 * A100147  Structured icosidodecahedral numbers.
 * https://oeis.org/A100147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(2*n-1)*(4*n-3)
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 813, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_s = (seq_t)n;
    seq_t term = n_s * (((seq_t)2 * n_s) - 1);
    term = term * (((seq_t)4 * n_s) - 3);
    return term;
}
