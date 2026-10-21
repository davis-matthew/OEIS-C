/*
 * A054054  Smallest digit of n.
 * https://oeis.org/A054054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest decimal digit of n (a(0) = 0)
 * Range:     u8 n <= 535, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t t = (seq_t)n;
    seq_t mn = 9;
    while (t > 0) {
        seq_t d = t % 10;
        if (d == 0) return 0;     /* zero digit → smallest */
        if (d < mn) mn = d;
        t /= 10;
    }
    return mn;
}
