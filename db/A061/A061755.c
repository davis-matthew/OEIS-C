/*
 * A061755  Positive integers k such that k! is divisible by (k+1)^7.
 * https://oeis.org/A061755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th positive integer k such that k! is divisible by (k+1)^7.
   The function searches k sequentially, testing the valuation condition
   for each prime dividing k+1.  Results are cached to make repeated calls
   efficient.  */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Types seq_t and idx_t are defined by the build system. */

static bool primes_inited = false;
static uint32_t *primes_arr = NULL;
static size_t primes_cnt = 0;

/* Simple sieve to generate all primes up to a fixed limit. */
static void init_primes(void)
{
    if (primes_inited)
        return;

    /* Fixed upper bound; sufficient for k up to several hundred thousand. */
    const idx_t limit = 2000000;
    char *is_prime = (char *)malloc((size_t)limit + 1);
    if (!is_prime)
        return;               /* allocation failed – give up */

    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* Count primes */
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++primes_cnt;

    primes_arr = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
    if (!primes_arr) {
        free(is_prime);
        return;
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            primes_arr[idx++] = (uint32_t)i;

    primes_inited = true;
    free(is_prime);
}

/* Returns true if k! is divisible by (k+1)^7. */
static bool good_k(idx_t k)
{
    init_primes();                /* Ensure the prime table exists */
    idx_t m = k + 1;              /* Number to test */
    idx_t tmp = m;

    for (size_t i = 0; i < primes_cnt && (idx_t)primes_arr[i] * (idx_t)primes_arr[i] <= tmp; ++i)
    {
        uint32_t p = primes_arr[i];
        if (tmp % p == 0)
        {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            /* Compute valuation of p in k! */
            idx_t val = 0;
            idx_t t = k;
            while (t) {
                t /= p;
                val += t;
            }
            if (val < 7 * e)
                return false;
        }
    }
    if (tmp > 1)          /* Remaining prime factor */
    {
        uint32_t p = (uint32_t)tmp;
        idx_t e = 1;
        idx_t val = 0;
        idx_t t = k;
        while (t) {
            t /= p;
            val += t;
        }
        if (val < 7 * e)
            return false;
    }
    return true;
}

/* Cache for the sequence values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_checked = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* Undefined for offset < 1 */

    while (cache_len < n)
    {
        ++last_checked;
        if (good_k(last_checked))
        {
            /* Grow cache if necessary */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    return 0;   /* Allocation failed; give up */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
