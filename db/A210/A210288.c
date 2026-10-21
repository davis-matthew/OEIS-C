/*
 * A210288  Number of 2 X 2 matrices with all elements in {0,1,...,n} and
 *          permanent = trace.
 * https://oeis.org/A210288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*n + 1 + 2*∑_{k=1..n} ⌊n/k⌋ (for n ≥ 2), with a(0) = 1,
 *            a(1) = 6
 * Range:     u8 n <= 20, u16 n <= 2939, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)6;

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        sum += (seq_t)(n / k);
    }

    seq_t result = 6 * (seq_t)n + 1 + 2 * sum;
    return result;
}
