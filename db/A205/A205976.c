/*
 * A205976  a(n) = Fibonacci(n)*A028594(n) for n>=1, with a(0)=1, where
 *          A028594 lists the coefficients in
 *          (theta_3(x)*theta_3(7*x)+theta_2(x)*theta_2(7*x))^2.
 * https://oeis.org/A205976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n) * A028594(n) (a(0)=1) A028594(n) = 4 *
 *            Σ_{d|n, gcd(d,7)=1} d = 4 * (σ(n) - 7*σ(n/7) if 7|n else σ(n))
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

static inline seq_t divisor_sum(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t q = n / d;
            sum += (seq_t)d;
            if (q != d)
                sum += (seq_t)q;
        }
    }
    return sum;
}

static seq_t A028594(idx_t n)
{
    seq_t sigma_n = divisor_sum(n);
    seq_t res;
    if (n % 7 == 0)
    {
        seq_t sigma_n7 = divisor_sum(n / 7);
        res = sigma_n - (seq_t)7 * sigma_n7;
    }
    else
    {
        res = sigma_n;
    }
    return (seq_t)4 * res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t fn = fib(n);          /* Fibonacci(n) */
    seq_t ck = A028594(n);      /* A028594(n)   */
    return fn * ck;             /* product      */
}
