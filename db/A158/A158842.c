/*
 * A158842  a(n) = 1 + n*(n+1)*(n-1)/2.
 * https://oeis.org/A158842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + n*(n+1)*(n-1)/2
 * Range:     u8 n <= 8, u16 n <= 50, u32 n <= 2048, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;

    seq_t t = (seq_t)n;

    seq_t tri;
    if ((t % 2) == 0)
        tri = (t / 2) * (t + 1);
    else
        tri = t * ((t + 1) / 2);

    return (seq_t)1 + tri * (t - 1);
}
