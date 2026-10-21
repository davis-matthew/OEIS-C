/*
 * A218446  a(n) = Sum_{k>=0} floor(n/(5*k + 3)).
 * https://oeis.org/A218446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k>=0} floor(n/(5*k + 3))
 * Range:     u8 n <= 252, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* iterate over k until the divisor exceeds n */
    for (idx_t k = 0;; ++k) {
        idx_t d = 5 * k + 3;
        if (d > n) break;
        sum += (seq_t)(n / d);
    }
    return sum;
}
