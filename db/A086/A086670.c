/*
 * A086670  Sum of floor(d/2) where d is a divisor of n.
 * https://oeis.org/A086670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n} floor(d/2)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            sum += (seq_t)(i / 2);
            idx_t j = n / i;
            if (j != i)
                sum += (seq_t)(j / 2);
        }
    }
    return sum;
}
