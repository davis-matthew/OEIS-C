/*
 * A078581  Arithmetic derivative of the average of n-th twin prime pair.
 * https://oeis.org/A078581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415(A014574(n))
 * Range:     u8 n <= 12, u16 n <= 276, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the arithmetic derivative of the average of the nth twin prime pair. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/*--- prime sieve initialization --------------------------------------------*/
static uint64_t *primes = NULL;
static size_t primes_len = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    const uint64_t LIMIT = 200000ULL;          /* enough for all searched numbers up to ~10^6   */
    bool *is_composite = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!is_composite) return;
    size_t cap = 15000;
    primes = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!primes) { free(is_composite); return; }
    for (uint64_t i = 2; i <= LIMIT; ++i)
    {
        if (!is_composite[i])
        {
            if (primes_len == cap)
            {
                cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, cap * sizeof(uint64_t));
                if (!tmp) { free(is_composite); return; }
                primes = tmp;
            }
            primes[primes_len++] = i;
            if (i * i <= LIMIT)
            {
                for (uint64_t j = i * i; j <= LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
    primes_inited = true;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < primes_len && primes[i] * primes[i] <= n; ++i)
        if (n % primes[i] == 0) return false;
    return true;
}

/*--- cache for averages of twin prime pairs --------------------------------*/
static uint64_t *twin_cache = NULL;
static size_t twin_cap = 0;
static size_t twin_len = 0;
static idx_t twin_next = 3ULL;   /* next odd to test for twin primes */

/* extend cache until it contains at least 'target' entries (1‑based) */
static void extend_cache(idx_t target)
{
    while (twin_len < (size_t)target)
    {
        /* find next twin prime pair */
        while (true)
        {
            if (is_prime_uint64(twin_next) && is_prime_uint64(twin_next + 2))
            {
                uint64_t avg = twin_next + 1;
                /* store */
                if (twin_len == twin_cap)
                {
                    twin_cap = twin_cap ? twin_cap * 2 : 16;
                    uint64_t *tmp = (uint64_t *)realloc(twin_cache, twin_cap * sizeof(uint64_t));
                    if (!tmp) return;  /* on OOM just stop */
                    twin_cache = tmp;
                }
                twin_cache[twin_len++] = avg;
                break;
            }
            twin_next += 2;
        }
        twin_next += 2; /* skip the used prime, start next search after it */
    }
}

/*--- arithmetic derivative of a positive integer --------------------------*/
static seq_t arithmetic_derivative(uint64_t m)
{
    if (m <= 1) return (seq_t)0;
    uint64_t original = m;
    uint64_t res = 0;
    uint64_t n = m;
    for (size_t i = 0; i < primes_len && primes[i] * primes[i] <= n; ++i)
    {
        uint64_t p = primes[i];
        if (n % p == 0)
        {
            uint64_t exp = 0;
            while (n % p == 0)
            {
                n /= p;
                ++exp;
            }
            res += (original / p) * exp;
        }
    }
    if (n > 1) /* remaining prime factor */
        res += original / n;
    return (seq_t)res;
}

/*--- main sequence function -------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    init_primes();          /* ensure prime table ready */
    extend_cache(n);        /* ensure we have the nth average */
    uint64_t avg = twin_cache[n - 1];
    return arithmetic_derivative(avg);
}
