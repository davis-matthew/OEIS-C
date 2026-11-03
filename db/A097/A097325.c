/*
 * A097325  Period 6: repeat [0, 1, 1, 1, 1, 1].
 * https://oeis.org/A097325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n % 6 == 0 else 1
 * Range:     u8 n <= 26244, u16 n <= 26244, u32 n <= 26244, u64 n <= 26244,
 *            u128 n <= 26244, big any n
 * Validated: standard, pass, 118 terms through n = 8692 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n % 6 == 0) ? (seq_t)0 : (seq_t)1;
}
