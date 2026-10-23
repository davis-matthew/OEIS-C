/*
 * A020475  a(n) is the number of k for which binomial(n,k) is divisible by n.
 * https://oeis.org/A020475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k, 0 <= k <= n, such that binomial(n,k) is
 *            divisible by n (equivalently a(n) = n + 1 - A007012(n))
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the exponent of prime p in n! (Legendre’s formula). */
static inline seq_t v_factorial(idx_t n, idx_t p)
{
    seq_t v = 0;
    for (idx_t div = p; div <= n; )
    {
        v += (seq_t)(n / div);
        if (div > n / p) break;        /* prevent overflow */
        div *= p;
    }
    return v;
}

/* Factorise m (m <= 64‑bit).  The arrays are large enough for all
   primes in the factorisation of numbers up to 10 000. */
static void factor_uint64(uint64_t m, uint64_t *primes, int *exps, int *cnt)
{
    *cnt = 0;
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            int e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            primes[(*cnt)] = p;
            exps[(*cnt)]  = e;
            ++(*cnt);
        }
    }
    if (m > 1)
    {
        primes[(*cnt)] = m;
        exps[(*cnt)]   = 1;
        ++(*cnt);
    }
}

/* OEIS A020475: number of k for which binomial(n,k) is divisible by n. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* definition of a(0) */

    uint64_t nn = (uint64_t)n;
    uint64_t primes[20];
    int       exps[20];
    int       prime_cnt;

    factor_uint64(nn, primes, exps, &prime_cnt);

    /* Pre‑compute v_p(n!) for all prime divisors of n. */
    seq_t v_n[20];
    for (int i = 0; i < prime_cnt; ++i)
        v_n[i] = v_factorial(n, primes[i]);

    seq_t count = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        bool divisible = true;
        for (int i = 0; i < prime_cnt; ++i)
        {
            seq_t val = v_n[i]
                       - v_factorial(k,        primes[i])
                       - v_factorial(n - k,    primes[i]);
            if (val < (seq_t)exps[i])   /* not enough p^e divisor */
            {
                divisible = false;
                break;
            }
        }
        if (divisible) ++count;
    }
    return count;
}
