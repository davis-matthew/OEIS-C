/*
 * A057826  Greatest number with totient 2n (or zero when no such number
 *          exists).
 * https://oeis.org/A057826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest m such that φ(m) = 2·n; returns 0 if no such m
 *            exists. The implementation precomputes φ values up to a fixed
 *            bound using a linear sieve, records for each φ the maximum m
 *            producing that value, and answers queries in O(1)
 * Range:     u8 n <= 35, u16 n <= 6719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are defined elsewhere by the build system */
seq_t a(idx_t n)
{
    /* Fixed bound for the sieve – large enough to cover typical queries.
       For ψ = φ(m) ≤ 2·n, m is at most about 5·ψ, so setting the bound to
       4,000,000 covers n up to roughly 2,000,000. */
    const int MAX_BOUND = 4000000;

    static int *phi = NULL;          /* linear sieve φ values */
    static seq_t *best = NULL;       /* best[φ] = max m with φ(m)=φ */
    static int *primes = NULL;       /* list of encountered primes */
    static size_t prime_cnt = 0, prime_cap = 0;
    static int initialized = 0;

    if (!initialized)
    {
        /* Allocate arrays */
        phi = (int *)malloc((MAX_BOUND + 1) * sizeof(int));
        best = (seq_t *)malloc((MAX_BOUND + 1) * sizeof(seq_t));
        prime_cap = 256;
        primes = (int *)malloc(prime_cap * sizeof(int));

        /* Linear sieve for φ */
        phi[0] = 0;
        phi[1] = 1;
        for (int i = 2; i <= MAX_BOUND; ++i)
        {
            if (phi[i] == 0)   /* i is prime */
            {
                phi[i] = i - 1;
                if (prime_cnt == prime_cap)
                {
                    prime_cap <<= 1;
                    primes = (int *)realloc(primes, prime_cap * sizeof(int));
                }
                primes[prime_cnt++] = i;
            }
            for (size_t j = 0; j < prime_cnt; ++j)
            {
                int p = primes[j];
                long long val = 1LL * i * p;
                if (val > MAX_BOUND)
                    break;
                if (i % p == 0)
                {
                    phi[(int)val] = phi[i] * p;
                    break;
                }
                else
                {
                    phi[(int)val] = phi[i] * (p - 1);
                }
            }
        }

        /* Compute best[φ] = maximum m */
        for (int i = 0; i <= MAX_BOUND; ++i)
            best[i] = (seq_t)0;

        for (int i = 1; i <= MAX_BOUND; ++i)
        {
            int k = phi[i];
            if (k <= MAX_BOUND && best[k] < (seq_t)i)
                best[k] = (seq_t)i;
        }

        initialized = 1;
    }

    /* Query */
    uint64_t target = 2ULL * n;
    if (target > (uint64_t)MAX_BOUND)
        return (seq_t)0; /* out of precomputed range */
    return best[(int)target];
}
