/*
 * A016844  a(n) = (4n+3)^8.
 * https://oeis.org/A016844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4n+3)^8
 * Range:     u8 none, u16 n <= 0, u32 n <= 3, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = ((seq_t)4 * (seq_t)n) + 3;
    seq_t res = base;
    for (idx_t i = 1; i < 8; ++i)
        res *= base;
    return res;
}
