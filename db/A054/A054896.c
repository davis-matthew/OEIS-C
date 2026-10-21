/*
 * A054896  a(n) = Sum_{k>0} floor(n/7^k).
 * https://oeis.org/A054896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>0} floor(n/7^k)
 * Range:     u8 n <= 1546, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t m = n;
    while (m) {
        m /= 7;                  /* floor(m/7) becomes floor(n/7^k) each step */
        sum += (seq_t)m;
    }
    return sum;
}
