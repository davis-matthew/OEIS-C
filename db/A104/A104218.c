/*
 * A104218  Sum of opposite numbers on a clock, starting at 12.
 * https://oeis.org/A104218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  full, nonn, fini
 * Formula:   a(n) = (n % 6 == 0 ? 18 : 2*(n % 6) + 6)
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 6;
    if (r == 0)
        return (seq_t)18;
    return (seq_t)(2 * r + 6);
}
