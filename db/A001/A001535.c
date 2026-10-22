/*
 * A001535  a(n) = (10n+1)*(10n+9).
 * https://oeis.org/A001535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10n+1)*(10n+9)
 * Range:     u8 n <= 1, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	seq_t nn = (seq_t)n;
	return (10 * nn + 1) * (10 * nn + 9);
}
