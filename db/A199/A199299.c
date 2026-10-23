/*
 * A199299  a(n) = (2*n + 1)*6^n.
 * https://oeis.org/A199299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n + 1)*6^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow6 = 1;
    seq_t base = 6;
    idx_t e = n;
    while (e > 0) {
        if (e & 1)
            pow6 = pow6 * base;
        base = base * base;
        e >>= 1;
    }
    seq_t factor = (seq_t)n;
    factor = factor * 2 + 1;
    return factor * pow6;
}
