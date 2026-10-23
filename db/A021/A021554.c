/*
 * A021554  Decimal expansion of 1/550.
 * https://oeis.org/A021554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = 0 for 0 ≤ n < 2; otherwise a(n) = 1 if (n‑2) is even,
 *            else 8. This yields the decimal digits of 1/550 after the
 *            decimal point
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    return ((n - 2) & 1) ? 8 : 1;
}
