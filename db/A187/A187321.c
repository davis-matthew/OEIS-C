/*
 * A187321  a(n) = floor(n/2) + floor(n/4).
 * https://oeis.org/A187321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/2) + floor(n/4)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 72 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t half   = ((seq_t)n) / 2;
    seq_t quarter = ((seq_t)n) / 4;
    return half + quarter;
}
