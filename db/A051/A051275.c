/*
 * A051275  Expansion of (1+x^2)/((1-x^2)*(1-x^3)).
 * https://oeis.org/A051275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor(n/2) + floor((n+4)/3) - n
 * Range:     u8 n <= 269, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nt = (seq_t)n;          /* cast index to seq_t for arithmetic */
    return (2 * (nt / 2)) + ((nt + 4) / 3) - nt;
}
