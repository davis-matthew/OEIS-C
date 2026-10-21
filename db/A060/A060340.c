/*
 * A060340  Possible sizes of the torsion group of an elliptic curve over some
 *          quadratic extension of the rationals Q. This is the full sequence.
 * https://oeis.org/A060340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = { n+1, for 0 ≤ n < 16; 18, for n = 16; 20, for n = 17;
 *            24, for n = 18; 0 otherwise }
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 16u)
        return (seq_t)(n + 1u);
    if (n == 16u)
        return (seq_t)18u;
    if (n == 17u)
        return (seq_t)20u;
    if (n == 18u)
        return (seq_t)24u;
    return (seq_t)0;
}
