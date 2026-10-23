/*
 * A013865  a(n) = 12^(5*n + 4).
 * https://oeis.org/A013865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12^(5*n + 4)
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 2, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t exp = 5*n + 4;
    seq_t res = (seq_t)1;
    seq_t base = (seq_t)12;
    for (idx_t i = 0; i < exp; ++i) {
        res *= base;
    }
    return res;
}
