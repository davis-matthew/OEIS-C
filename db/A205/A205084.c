/*
 * A205084  a(n) = n 4's sandwiched between two 1's.
 * https://oeis.org/A205084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base, less
 * Formula:   a(0) = 11, a(n) = 10*a(n-1) + 31 (n >= 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 11;
    for (idx_t i = 1; i <= n; ++i) {
        val = ((seq_t)10 * val) + (seq_t)31;
    }
    return val;
}
