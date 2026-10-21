/*
 * A070512  a(n) = n^4 mod 7.
 * https://oeis.org/A070512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 7
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t tbl[7] = { (seq_t)0, (seq_t)1, (seq_t)2, (seq_t)4,
                                 (seq_t)4, (seq_t)2, (seq_t)1 };
    idx_t m = n % 7;
    return tbl[m];
}
