/*
 * A102716  Triangle read by rows: T(n,k) = sigma(binomial(n,k)) (0 <= k <=
 *          n), where sigma(m) is the sum of the positive divisors of m.
 * https://oeis.org/A102716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sigma(binomial(row,col)) * where the triangle
 *            T(m,k)=sigma(binomial(m,k)) is flattened row by row
 * Range:     u8 n <= 48, u16 n <= 177, u32 n <= 608, u64 n <= 2239,
 *            u128 n <= 5150, big any n
 * Validated: standard, pass, 90 terms through n = 5137 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* exponent of prime p in m! */
static uint32_t exp_factorial(idx_t m, uint32_t p)
{
    uint32_t e = 0;
    idx_t t = m;
    while (t)
    {
        t /= p;
        e += t;
    }
    return e;
}

/* fast exponentiation: base^exp (base is small, 32‑bit integer) */
static seq_t pow_uint32(uint32_t base, uint32_t exp)
{
    seq_t result = 1;
    seq_t b = (seq_t)base;
    while (exp)
    {
        if (exp & 1)
            result *= b;
        exp >>= 1;
        if (exp)
            b *= b;
    }
    return result;
}

/* sigma(binomial(n,k)) */
static seq_t sigma_binom(idx_t n, idx_t k)
{
    if (k == 0 || k == n)
        return 1;                     /* sigma(1) = 1 */

    seq_t sigma = 1;

    /* trial division for primes up to n (sufficient for n <= ~2000) */
    for (uint32_t p = 2; p <= n; ++p)
    {
        /* primality test */
        bool prime = true;
        for (uint32_t d = 2; d <= p / d; ++d)
        {
            if (p % d == 0)
            {
                prime = false;
                break;
            }
        }
        if (!prime)
            continue;

        /* exponent of p in binomial(n,k) */
        uint32_t e = exp_factorial(n, p);
        e -= exp_factorial(k, p);
        e -= exp_factorial(n - k, p);
        if (e == 0)
            continue;

        /* term = (p^(e+1) - 1) / (p - 1) */
        seq_t numerator   = pow_uint32(p, e + 1) - 1;
        seq_t denominator = (seq_t)(p - 1);
        seq_t term = numerator / denominator;
        sigma *= term;
    }

    return sigma;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* find row r and column k such that
       cumulative = sum_{i=0}^{r-1} (i+1) <= n < sum_{i=0}^{r} (i+1) */
    idx_t cumulative = 0;
    idx_t r = 0;
    for (;; ++r)
    {
        idx_t next = cumulative + (r + 1);
        if (n < next)
            break;
        cumulative = next;
    }
    idx_t k = n - cumulative;
    return sigma_binom(r, k);
}
