/*
 * A054900  a(n) = Sum_{j >= 1} floor(n/16^j).
 * https://oeis.org/A054900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j >= 1} floor(n / 16^j)
 * Range:     u8 n <= 3855, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t t = n;
    while (t >= 16) {
        t /= 16;
        sum += (seq_t)t;
    }
    return sum;
}
