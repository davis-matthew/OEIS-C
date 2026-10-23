/*
 * A048681  Maximum over k of the largest squarefree number dividing a value
 *          of binomial(n,k).
 * https://oeis.org/A048681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{0≤k≤n} rad(binomial(n,k)) (rad(x) = product of the
 *            distinct prime divisors of x)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* static data for caching primes and factorial prime exponents */
static idx_t max_n           = 0;          /* largest n processed so far     */
static uint32_t *primes      = NULL;       /* primes ≤ max_n                  */
static size_t   primes_len   = 0;          /* number of primes in primes[]    */
static uint16_t **pfacts     = NULL;       /* pfacts[pi][i] = exp_p(i!)      */
static size_t   pfacts_len   = 0;          /* equal to primes_len             */

/* free old cache */
static void free_cache(void)
{
    if (pfacts) {
        for (size_t i = 0; i < pfacts_len; ++i)
            free(pfacts[i]);
        free(pfacts);
        pfacts = NULL;
    }
    if (primes) {
        free(primes);
        primes = NULL;
    }
    pfacts_len = 0;
    primes_len = 0;
    max_n = 0;
}

/* rebuild tables for all n' ≤ limit */
static void rebuild_tables(idx_t limit)
{
    free_cache();

    /* 1. sieve primes up to limit */
    size_t L = (size_t)limit + 1;
    bool *is_prime = (bool *)malloc(L);
    if (!is_prime) exit(1);
    memset(is_prime, true, L);
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; (idx_t)(i * i) <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* count primes */
    size_t pc = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++pc;

    primes      = (uint32_t *)malloc(pc * sizeof(uint32_t));
    if (!primes) exit(1);
    primes_len  = pc;
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[pos++] = (uint32_t)i;

    free(is_prime);

    /* 2. allocate pfacts arrays */
    pfacts     = (uint16_t **)malloc(primes_len * sizeof(uint16_t *));
    if (!pfacts) exit(1);
    pfacts_len = primes_len;
    for (size_t i = 0; i < pfacts_len; ++i)
        pfacts[i] = NULL;

    /* 3. compute prefix exponents for each prime */
    for (size_t pi = 0; pi < primes_len; ++pi) {
        uint32_t p = primes[pi];
        uint16_t *arr = (uint16_t *)malloc((limit + 1) * sizeof(uint16_t));
        if (!arr) exit(1);
        arr[0] = 0;
        for (idx_t i = 1; i <= limit; ++i) {
            idx_t t = i;
            uint16_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            arr[i] = arr[i - 1] + e;
        }
        pfacts[pi] = arr;
    }

    max_n = limit;
}

/* ensure tables cover n */
static void ensure_tables(idx_t n)
{
    if (n > max_n)
        rebuild_tables(n);
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)               /* offset is 1, below it the value is irrelevant */
        return (seq_t)1;

    ensure_tables(n);

    seq_t best = (seq_t)1;   /* at least 1 */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t rad = (seq_t)1;
        idx_t nk = n - k;
        for (size_t pi = 0; pi < primes_len; ++pi) {
            const uint16_t *arr = pfacts[pi];
            uint16_t exp_n   = arr[n];
            uint16_t exp_k   = arr[k];
            uint16_t exp_nk  = arr[nk];
            if (exp_n > exp_k + exp_nk) {
                rad *= (seq_t)primes[pi];
            }
        }
        if (rad > best)
            best = rad;
    }

    return best;
}
