/*
 * A054895  a(n) = Sum_{k>0} floor(n/6^k).
 * https://oeis.org/A054895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>0} floor(n/6^k)
 * Range:     u8 n <= 1295, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t div = n / 6;          /* floor(n/6) */
    seq_t sum = 0;
    while (div > 0) {
        sum += (seq_t)div;      /* add floor(n/6^k) */
        div /= 6;               /* next power of 6 */
    }
    return sum;
}
