/*
 * A093907  Number of elements in the n-th period of the periodic table as
 *          predicted by the Aufbau principle.
 * https://oeis.org/A093907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * floor((n+2)/2)^2
 * Range:     u8 n <= 21, u16 n <= 361, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 69 terms through n = 8153 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = ((seq_t)n + 2) / 2;
    return 2 * m * m;
}
