/*
 * A094177  Sequence from an aptitude test that I cannot work out!
 * https://oeis.org/A094177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, unkn, bref
 * Formula:   a(n) = 3 + 3*(n-2)*2^(n-4) for n >= 4; a(1) = 4, a(2) = 3, a(3)
 *            = 4
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)4;
    seq_t v = (seq_t)3 * ((seq_t)n - 2);
    for (idx_t i = 0; i < n - 4; ++i) v *= 2;
    return v + 3;
}
