/*
 * A157369  a(n) = 343*n - 273.
 * https://oeis.org/A157369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 343*n - 273
 * Range:     u8 n <= 1, u16 n <= 191, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v = (seq_t)n;
    v = v * (seq_t)343 - (seq_t)273;
    return v;
}
