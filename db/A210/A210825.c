/*
 * A210825  G.f.: Sum_{n>=1} a(n)*x^n/(1-x^n) = x*Sum_{n>=1} x^(n*(n-1)/2).
 * https://oeis.org/A210825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{d|n} μ(d) · b(n/d), where μ is the Möbius function and
 *            b(m) = 1 if 8·m – 7 is a perfect square and that square is odd,
 *            else b(m) = 0
 * Range:     i8 n <= 1035, i16 n <= 1035, i32 n <= 1035, i64 n <= 1035,
 *            i128 n <= 1035, big any n
 * Helpers:   is_triangular_plus_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_triangular_plus_one.h"

/* Returns 1, -1 or 0 according to the Möbius function μ for d ≥ 1. */
static seq_t mu_of(idx_t d)
{
    if (d == 1)
        return (seq_t)1;

    idx_t temp = d;
    int cnt = 0;           /* number of distinct prime factors      */
    bool sq_factor = false;/* true if a squared prime factor occurs */

    /* iterate over prime divisors up to sqrt(temp) */
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            int exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
                if (exp > 1)
                {
                    sq_factor = true;
                    break;
                }
            }
            if (sq_factor)
                break;
            ++cnt;        /* distinct prime factor */
        }
    }

    /* if a remaining prime factor > 1, it contributes one more factor */
    if (!sq_factor && temp > 1)
        ++cnt;

    if (sq_factor)
        return (seq_t)0;

    return (cnt & 1) ? (seq_t)(-1) : (seq_t)1;
}

/* Sequence a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* out of bounds guard */

    seq_t acc = 0;

    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i != 0)
            continue;

        /* divisor d = i */
        idx_t d = i;
        seq_t mu = mu_of(d);
        if (mu != 0)
        {
            idx_t m = n / d;
            acc += mu * (seq_t)is_triangular_plus_one(m);
        }

        /* paired divisor d2 = n / i, if distinct */
        idx_t d2 = n / i;
        if (d2 != d)
        {
            seq_t mu2 = mu_of(d2);
            if (mu2 != 0)
            {
                idx_t m2 = n / d2;  /* equals d */
                acc += mu2 * (seq_t)is_triangular_plus_one(m2);
            }
        }
    }
    return acc;
}
