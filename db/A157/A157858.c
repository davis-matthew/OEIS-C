/*
 * A157858  a(n) = 1728000*n - 240.
 * https://oeis.org/A157858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1728000*n - 240
 * Range:     u8 none, u16 none, u32 n <= 2485, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 38 terms through n = 9324 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = ((seq_t)n) * ((seq_t)1728000u);
    val -= ((seq_t)240u);
    return val;
}
