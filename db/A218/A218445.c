/*
 * A218445  a(n) = Sum_{k>=0} floor(n/(5*k + 2)).
 * https://oeis.org/A218445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k>=0} floor(n/(5*k + 2))
 * Range:     u8 n <= 215, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; ; ++k) {
        idx_t denom = 5 * k + 2;
        if (denom > n) break;
        sum += (seq_t)(n / denom);
    }
    return sum;
}
