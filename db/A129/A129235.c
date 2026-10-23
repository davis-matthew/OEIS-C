/*
 * A129235  a(n) = 2*sigma(n) - tau(n), where tau(n) is the number of divisors
 *          of n (A000005) and sigma(n) is the sum of divisors of n (A000203).
 * https://oeis.org/A129235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*sigma(n) - tau(n) (sigma = sum of divisors, tau =
 *            divisor count)
 * Range:     u8 n <= 16, u16 n <= 1796, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static state: smallest prime factor array */
static idx_t *spf = NULL;
static idx_t spf_limit = 0;

/* Build an SPF (smallest prime factor) table up to 'limit'. */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit)
        return;
    free(spf);
    spf = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (!spf)
        return; /* allocation failure; caller will work with incomplete data */
    for (idx_t i = 0; i <= limit; ++i)
        spf[i] = i; /* initial assumption: each number is its own factor */

    for (idx_t i = 2; i * i <= limit; ++i)
    {
        if (spf[i] == i)           /* i is prime */
        {
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == j)
                    spf[j] = i;     /* set smallest prime factor */
        }
    }
    spf_limit = limit;
}

/* Compute a(n) = 2*sigma(n) - tau(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* outside OEIS offset; safe fallback */

    ensure_spf(n);

    idx_t x = n;
    seq_t sigma  = 1;
    seq_t tau    = 1;

    while (x > 1)
    {
        idx_t p = spf[x];
        idx_t e = 0;
        while (x % p == 0)
        {
            x /= p;
            ++e;
        }

        tau  *= (seq_t)(e + 1);

        /* compute (p^(e+1) - 1) / (p - 1) */
        seq_t p_pow = 1;
        for (idx_t i = 0; i < e + 1; ++i)
            p_pow *= (seq_t)p;
        seq_t numerator = p_pow - 1;
        seq_t denominator = (seq_t)p - 1;
        seq_t term = numerator / denominator;
        sigma *= term;
    }

    return sigma * 2 - tau;
}
