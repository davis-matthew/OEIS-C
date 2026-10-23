/*
 * A174937  a(n) = Sum_{d|n} d^tau(d).
 * https://oeis.org/A174937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^tau(d) where tau is the divisor-count
 *            function
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 59, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast exponentiation: base^exp */
static inline seq_t pow_seq(seq_t base, uint64_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1ULL) result *= base;
        exp >>= 1ULL;
        if (exp) base *= base;
    }
    return result;
}

/* Number of divisors of x (x <= 10000 in expected use) */
static inline uint64_t tau_uint64(uint64_t x)
{
    uint64_t res = 1;
    for (uint64_t p = 2; p * p <= x; ++p)
    {
        if (x % p == 0)
        {
            uint64_t cnt = 0;
            while (x % p == 0)
            {
                x /= p;
                ++cnt;
            }
            res *= (cnt + 1);
        }
    }
    if (x > 1) res *= 2;
    return res;
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    seq_t sum = 0;
    for (uint64_t d = 1; d * d <= nn; ++d)
    {
        if (nn % d == 0)
        {
            uint64_t tau_d = tau_uint64(d);
            sum += pow_seq((seq_t)d, tau_d);

            uint64_t other = nn / d;
            if (other != d)
            {
                uint64_t tau_o = tau_uint64(other);
                sum += pow_seq((seq_t)other, tau_o);
            }
        }
    }
    return sum;
}
