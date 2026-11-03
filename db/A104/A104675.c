/*
 * A104675  a(n) = C(n+1,n) * C(n+6,1).
 * https://oeis.org/A104675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n + 1) * (n + 6)
 * Range:     u8 n <= 12, u16 n <= 252, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 912 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n + 1) * ((seq_t)n + 6);
}
