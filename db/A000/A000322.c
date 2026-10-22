/*
 * A000322  Pentanacci numbers: a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) +
 *          a(n-5) with a(0) = a(1) = a(2) = a(3) = a(4) = 1.
 * https://oeis.org/A000322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) + a(n-5), a(0) = a(1) =
 *            a(2) = a(3) = a(4) = 1
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 68, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t w0 = 1, w1 = 1, w2 = 1, w3 = 1, w4 = 1;
    idx_t i;
    if (n < 5)
        return (seq_t)1;
    for (i = 5; i <= n; i++) {
        seq_t next = w0 + w1 + w2 + w3 + w4;
        w0 = w1;
        w1 = w2;
        w2 = w3;
        w3 = w4;
        w4 = next;
    }
    return w4;
}
