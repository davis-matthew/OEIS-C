/*
 * A068317  Numbers n such that n*tau(n) > omega(n)*prime(n) where tau(n) is
 *          the number of divisors of n and omega(n) the number of distinct
 *          prime factors of n.
 * https://oeis.org/A068317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Numbers n such that n*tau(n) > omega(n)*prime(n)
 * Range:     u8 n <= 9, u16 n <= 859, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* prime(n) is the n-th prime number. This implementation
 * builds the sequence lazily. It precomputes all primes up
 * to a fixed limit (15 million) using a simple sieve.
 * Each candidate n is tested by factoring it via trial
 * division with the prime list. The sequence terms are
 * cached, so subsequent calls are O(1). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --- global data --------------------------------------------------------- */
#define PRIME_LIMIT 15000000u          /* upper bound for prime generation   */
static uint32_t *primes = NULL;        /* array of primes, 1‑based index     */
static size_t prime_cnt = 0;           /* number of primes stored           */

static seq_t *seq_cache = NULL;       /* computed sequence terms            */
static size_t seq_len  = 0;            /* terms stored so far                */
static size_t seq_cap  = 0;            /* allocated capacity for seq_cache   */
static idx_t  last_checked = 0;        /* largest integer n already tested   */

/* -------------------------------------------------------------------------- */
/* Build the prime list once up to PRIME_LIMIT. The list is stored in
   'primes', zero‑based.  The array contains all primes <= PRIME_LIMIT      */
static void build_primes(void)
{
    if (primes != NULL) return;                 /* already built */
    unsigned char *sieve = (unsigned char*)calloc(PRIME_LIMIT + 1, 1);
    if (!sieve) return;                         /* allocation failed – should not happen */

    /* reserve space for ≈ limit / log(limit) primes */
    size_t est = PRIME_LIMIT / 10 + 1000;
    primes = (uint32_t*)malloc(est * sizeof(uint32_t));
    prime_cnt = 0;

    for (idx_t i = 2; i <= PRIME_LIMIT; ++i)
    {
        if (!sieve[i])
        {
            if (prime_cnt == est)
            {
                est += 50000;                 /* grow buffer a bit */
                primes = (uint32_t*)realloc(primes, est * sizeof(uint32_t));
            }
            primes[prime_cnt++] = (uint32_t)i;
            if ((uint64_t)i * i <= PRIME_LIMIT)
                for (idx_t j = i * i; j <= PRIME_LIMIT; j += i)
                    sieve[j] = 1;
        }
    }
    free(sieve);
}

/* Append a new term to the cache, growing the buffer if necessary. */
static void cache_term(seq_t val)
{
    if (seq_len == seq_cap)
    {
        seq_cap = seq_cap ? seq_cap << 1 : 64;
        seq_cache = (seq_t*)realloc(seq_cache, seq_cap * sizeof(seq_t));
    }
    seq_cache[seq_len++] = val;
}

/* Check whether integer n satisfies n*tau(n) > omega(n)*prime(n). */
static bool satisfies(idx_t n)
{
    idx_t tmp = n;
    seq_t tau = 1;
    idx_t omega = 0;

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i]*primes[i] <= tmp; ++i)
    {
        uint32_t p = primes[i];
        if (tmp % p == 0)
        {
            ++omega;
            int cnt = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++cnt;
            }
            tau *= (cnt + 1);
        }
    }
    if (tmp > 1)                /* remaining prime factor */
    {
        ++omega;
        tau *= 2;
    }

    seq_t lhs = (seq_t)n * tau;
    seq_t rhs = (seq_t)omega * (seq_t)primes[n - 1];   /* nth prime, 1‑based */
    return lhs > rhs;
}

/* -------------------------------------------------------------------------- */
/* Main function as required by the interface.                                 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of bounds – not used by the tests */

    build_primes();                  /* ensure primes array is ready */

    while (seq_len < n)
    {
        ++last_checked;
        if (satisfies(last_checked))
            cache_term((seq_t)last_checked);
    }
    return seq_cache[n - 1];
}
