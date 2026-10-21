/*
 * A099168  a(n) = 3^n * 5^binomial(n,2).
 * https://oeis.org/A099168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^n * 5^(n*(n-1)/2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t pow3 = pow_seq_t((seq_t)3, n);
    idx_t exp5 = n * (n - 1) / 2;
    seq_t pow5  = pow_seq_t((seq_t)5, exp5);
    return pow3 * pow5;
}
