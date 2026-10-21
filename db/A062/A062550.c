/*
 * A062550  a(n) = Sum_{k = 1..2n} floor(2n/k).
 * https://oeis.org/A062550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..2n} floor(2n/k)
 * Range:     u8 n <= 29, u16 n <= 3623, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * 2;   /* 2n */
    seq_t sum = 0;
    seq_t k = 1;

    while (k <= m) {
        seq_t v = m / k;            /* floor(m/k) */
        seq_t next_k = m / v + 1;   /* first k where floor(m/k) changes */
        sum += v * (next_k - k);
        k = next_k;
    }
    return sum;
}
