/*
 * A153485  Sum of all aliquot divisors of all positive integers <= n.
 * https://oeis.org/A153485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} k * floor((n - k)/k)
 * Range:     u8 n <= 29, u16 n <= 450, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t q = n / k;          /* floor(n/k) */
        if (q <= 1)                /* no contribution when q-1 == 0 */
            continue;
        sum += (seq_t)k * (seq_t)(q - 1);
    }
    return sum;
}
