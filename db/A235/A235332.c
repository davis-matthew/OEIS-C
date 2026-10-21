/*
 * A235332  a(n) = n*(9*n + 25)/2 + 6.
 * https://oeis.org/A235332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(9*n + 25)/2 + 6, offset 0
 * Range:     u8 n <= 4, u16 n <= 118, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the unsigned index to the output type before any arithmetic */
    seq_t nn = (seq_t)n;
    seq_t term = nn * ((nn * (seq_t)9) + (seq_t)25);
    return term / (seq_t)2 + (seq_t)6;
}
