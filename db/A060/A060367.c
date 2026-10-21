/*
 * A060367  Average order of an element in a cyclic group of order n rounded
 *          down.
 * https://oeis.org/A060367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( σ₂(n²) / ( n · σ(n²) ) ) where σ₂(m) = ∑_{d|m} d²
 *            (sum of squares of divisors) and σ(m) = ∑_{d|m} d (sum of
 *            divisors)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* 128‑bit power:  pᵉ  (e may be 0) */
static unsigned __int128 pow128(unsigned __int128 p, unsigned e)
{
    unsigned __int128 r = 1;
    while (e)
    {
        if (e & 1u) r *= p;
        p *= p;
        e >>= 1u;
    }
    return r;
}

/* factorise n into primes[0..cnt‑1] and exps[0..cnt‑1] */
static void factorise(uint64_t n, uint64_t primes[], int exps[], int *cnt)
{
    *cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            int e = 0;
            while (n % p == 0)
            {
                n /= p;
                ++e;
            }
            primes[(*cnt)] = p;
            exps[(*cnt)]   = e;
            ++(*cnt);
        }
    }
    if (n > 1)
    {
        primes[(*cnt)] = n;
        exps[(*cnt)]   = 1;
        ++(*cnt);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* sequence starts at n = 1 */

    uint64_t nn = (uint64_t)n;

    /* factor n */
    uint64_t primes[64];
    int exps[64];
    int cnt;
    factorise(nn, primes, exps, &cnt);

    /* compute σ(n²) and σ₂(n²) using 128‑bit arithmetic */
    unsigned __int128 sigma_n2      = 1;
    unsigned __int128 sigma2_n_sq   = 1;

    for (int i = 0; i < cnt; ++i)
    {
        unsigned __int128 p  = primes[i];
        int            e    = exps[i];

        /* σ(n²):   (p^{2e+1} − 1)/(p − 1) */
        unsigned __int128 num_n2  = pow128(p, 2 * e + 1) - (unsigned __int128)1;
        unsigned __int128 den_n2  = p - (unsigned __int128)1;
        sigma_n2 *= num_n2 / den_n2;

        /* σ₂(n²): (p^{4e+2} − 1)/(p² − 1) */
        unsigned __int128 num2    = pow128(p, 4 * e + 2) - (unsigned __int128)1;
        unsigned __int128 den2    = p * p - (unsigned __int128)1;
        sigma2_n_sq *= num2 / den2;
    }

    unsigned __int128 numerator   = sigma2_n_sq;
    unsigned __int128 denominator = (unsigned __int128)nn * sigma_n2;

    unsigned __int128 result128 = numerator / denominator;  /* floor division */

    return (seq_t)result128;
}
