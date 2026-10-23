/*
 * A129468  Unitary abundance of n.
 * https://oeis.org/A129468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = σ*(n) - 2*n where σ*(n) = product over prime powers (1 +
 *            p^e) for each p^e || n (the sum of unitary divisors of n)
 * Range:     i8 n <= 130, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 1;
    idx_t m = n;

    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            idx_t pe = 1;
            while (m % p == 0)
            {
                pe *= p;
                m /= p;
            }
            sum *= (seq_t)(1 + pe);
        }
    }

    if (m > 1)
    {
        sum *= (seq_t)(1 + m);
    }

    return sum - (seq_t)n * 2;
}
