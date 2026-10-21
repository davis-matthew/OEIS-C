/*
 * A061391  a(n) = t(n,3) = Sum_{d|n} tau(d^3), where tau(n) = number of
 *          divisors of n, cf. A000005.
 * https://oeis.org/A061391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} tau(d^3) (t(n,3))
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const uint32_t MAX_PRIME = 1000000;   /* Sieve limit: sqrt(10^12) */
static uint32_t *primes = NULL;
static size_t prime_cnt = 0;
static int sieve_done = 0;

/* Simple sieve of Eratosthenes filling the primes array */
static void init_sieve(void)
{
    size_t i, j;
    uint8_t *is_composite = NULL;
    size_t limit = MAX_PRIME;
    size_t sz = limit + 1;
    is_composite = (uint8_t*)calloc(sz, 1);
    if (!is_composite) return;   /* OOM: fallback to trial division */

    primes = (uint32_t*)malloc(sz * sizeof(uint32_t)); /* over‑allocate */
    if (!primes) return;          /* OOM */

    for (i = 2; i <= limit; ++i)
    {
        if (!is_composite[i])
        {
            primes[prime_cnt++] = (uint32_t)i;
            if (i * i <= limit)
                for (j = i * i; j <= limit; j += i)
                    is_composite[j] = 1;
        }
    }
    sieve_done = 1;
    free(is_composite);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* defined only for n>=1, but guard */

    /* Ensure primes are available */
    if (!sieve_done) init_sieve();

    idx_t temp = n;
    seq_t result = (seq_t)1;

    /* Factor out primes up to sqrt(temp) */
    size_t pi = 0;
    while (pi < prime_cnt && ((uint64_t)primes[pi] * primes[pi] <= temp))
    {
        uint32_t p = primes[pi++];
        if (temp % p == 0)
        {
            idx_t exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            /* part = ((3*exp + 2)*(exp + 1)) / 2 */
            seq_t part = (seq_t)((3 * exp + 2) * (exp + 1)) / 2;
            result *= part;
        }
    }

    /* Remaining prime factor (if any) */
    if (temp > 1)
    {
        idx_t exp = 1;
        seq_t part = (seq_t)((3 * exp + 2) * (exp + 1)) / 2;
        result *= part;
    }

    return result;
}
