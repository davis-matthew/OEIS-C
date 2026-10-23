/*
 * A016883  a(n) = (5*n + 2)^11.
 * https://oeis.org/A016883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 2)^11
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 10, u128 n <= 636,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)5 * (seq_t)n + 2;
    seq_t res = 1;
    int e = 11;
    while (e) {
        if (e & 1)
            res *= x;
        x *= x;
        e >>= 1;
    }
    return res;
}
