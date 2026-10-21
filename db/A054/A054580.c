/*
 * A054580  n^2 modulo 17.
 * https://oeis.org/A054580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 17
 * Range:     u8 n <= 15, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = (seq_t)17;
    seq_t val = (seq_t)n * (seq_t)n;
    return val % MOD;
}
