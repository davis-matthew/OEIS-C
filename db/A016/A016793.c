/*
 * A016793  a(n) = (3*n + 2)^5.
 * https://oeis.org/A016793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n + 2)^5
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 27, u64 n <= 2376,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)3 * n + (seq_t)2;
    seq_t res = base;
    for (int i = 1; i < 5; ++i)
        res *= base;
    return res;
}
