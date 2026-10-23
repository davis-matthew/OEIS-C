/*
 * A198301  G.f.: exp( Sum_{n>=1} (x^n/n) * Sum_{d|n} d*sigma(n/d,d) ).
 * https://oeis.org/A198301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 32, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A198301
 * G.f.: exp( Sum_{n>=1} (x^n/n) * Sum_{d|n} d * sigma(n/d,d) )
 * a(0) = 1
 * Recurrence: a(n) = (1/n) * sum_{k=1..n} a(n-k) * B(k)
 * where B(k) = sum_{d|k} d * sigma(k/d,d)
 * and sigma(m,r) = sum_{t|m} t^r
 */

#include <stdlib.h>
#include <stdint.h>

/* fast exponentiation: base^exp  (base and result are of type seq_t) */
static seq_t seq_pow(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* sum of the r-th powers of all divisors of n  (n>=1) */
static seq_t sigma_power(idx_t n, idx_t r)
{
    if (n == 1) return 1;

    seq_t result = 1;
    idx_t tmp = n;

    /* factor n by trial division */
    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p) continue;
        idx_t exp = 0;
        while (tmp % p == 0)
        {
            tmp /= p;
            ++exp;
        }

        seq_t p_pow_r = seq_pow((seq_t)p, r);                     /* p^r            */
        seq_t numerator   = seq_pow(p_pow_r, exp + 1) - 1;        /* (p^r)^(exp+1)-1 */
        seq_t denominator = p_pow_r - 1;                          /* p^r - 1        */
        seq_t term = numerator / denominator;                     /* sum of p^(i*r) */
        result *= term;
    }

    /* remaining prime factor (if any) */
    if (tmp > 1)
    {
        idx_t exp = 1;
        seq_t p_pow_r = seq_pow((seq_t)tmp, r);
        seq_t numerator   = seq_pow(p_pow_r, 2) - 1;            /* (p^r)^2-1 */
        seq_t denominator = p_pow_r - 1;                       /* p^r-1   */
        seq_t term = numerator / denominator;
        result *= term;
    }

    return result;
}

/* memoized computation of a(n) */
seq_t a(idx_t n)
{
    /* static arrays for memoisation */
    static seq_t *A = NULL;
    static seq_t *B = NULL;
    static idx_t max_computed = 0;

    /* base case */
    if (n == 0)
    {
        if (!A)
        {
            A = (seq_t*)malloc(sizeof(seq_t));
            B = (seq_t*)malloc(sizeof(seq_t));
            A[0] = 1;
            B[0] = 0;
            max_computed = 0;
        }
        return A[0];
    }

    /* enlarge tables if necessary */
    if (n > max_computed)
    {
        A = (seq_t*)realloc(A, (n + 1) * sizeof(seq_t));
        B = (seq_t*)realloc(B, (n + 1) * sizeof(seq_t));

        /* compute B for new indices */
        for (idx_t m = max_computed + 1; m <= n; ++m)
        {
            seq_t sum = 0;
            for (idx_t d = 1; d <= m / d; ++d)
            {
                if (m % d) continue;
                idx_t d2 = m / d;

                seq_t sigma1 = sigma_power(m / d, d);
                sum += (seq_t)d * sigma1;

                if (d2 != d)
                {
                    seq_t sigma2 = sigma_power(d, d2);
                    sum += (seq_t)d2 * sigma2;
                }
            }
            B[m] = sum;
        }

        /* compute A for new indices using recurrence */
        for (idx_t i = max_computed + 1; i <= n; ++i)
        {
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k)
            {
                sum += A[i - k] * B[k];
            }
            A[i] = sum / (seq_t)i;   /* exact division */
        }

        max_computed = n;
    }

    return A[n];
}
