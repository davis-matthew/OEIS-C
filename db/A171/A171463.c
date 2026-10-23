/*
 * A171463  The natural numbers excluding 2 and 3.
 * https://oeis.org/A171463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + [n>1]*2
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)n;
    } else {
        return (seq_t)n + 2;
    }
}
