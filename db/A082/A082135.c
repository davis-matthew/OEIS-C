/*
 * A082135  Expansion of e.g.f. x*exp(4*x)*cosh(x).
 * https://oeis.org/A082135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(3^(n-1)+5^(n-1))/2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t power_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t sn = (seq_t)n;
    seq_t pow3 = power_seq_t((seq_t)3, n - 1);
    seq_t pow5 = power_seq_t((seq_t)5, n - 1);
    seq_t sum = pow3 + pow5;
    seq_t prod = sn * sum;
    return prod / 2;
}
