/*
 * A067351  Numbers k such that sigma(k) + phi(k) has exactly 2 distinct prime
 *          divisors.
 * https://oeis.org/A067351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that sigma(k) + phi(k) has exactly two distinct
 *            prime divisors
 * Range:     u8 n <= 166, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 86 terms through n = 997 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#include <stdbool.h>

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Compute Euler's totient φ of an odd 64‑bit or larger number m.
   The algorithm uses trial division up to sqrt(m). */
static seq_t calc_phi(seq_t m)
{
    seq_t result = m;
    seq_t p = 2;

    /* Handle factor 2 separately to skip it later (m is odd, but keep robust) */
    if (m % 2 == 0) {
        while (m % 2 == 0)
            m /= 2;
        result -= result / 2;
    }

    /* Trial division by odd primes */
    for (p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            while (m % p == 0)
                m /= p;
            result -= result / p;
        }
    }

    /* If m > 1, it is a prime factor larger than sqrt(original m) */
    if (m > 1)
        result -= result / m;

    return result;
}

/* Count distinct prime factors of an unsigned 64-bit integer n.
   The result fits in a signed int for all n that may be passed. */
static int count_distinct_primes(uint64_t n)
{
    if (n <= 1)
        return 0;

    int cnt = 0;
    /* Handle factor 2 separately. */
    if (n % 2 == 0) {
        cnt++;
        while (n % 2 == 0)
            n /= 2;
    }
    /* Test odd divisors up to sqrt(n). */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            cnt++;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1)   /* Remaining prime factor. */
        cnt++;
    return cnt;
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;   /* Array of accepted k values */
static size_t cache_len = 0;  /* Number of elements stored */
static size_t cache_cap = 0;  /* Capacity of the array */
static idx_t  last_k   = 0;   /* Last k that was examined */

/* Ensure the cache can hold at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    if (!tmp)
        abort();
    cache      = tmp;
    cache_cap  = new_cap;
}

/* Main sequence function.  Returns the n‑th member of A067351.
   n is 1‑based; values for n == 0 are undefined. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* offset 1 – caller should never request 0 */

    /* Build the cache until we have at least n elements. */
    while (cache_len < n)
    {
        /* Next candidate k */
        last_k++;

        /* Compute sigma(k) + phi(k). */
        seq_t sigma = sigma_of(last_k);
        seq_t phi   = calc_phi((seq_t)last_k);
        seq_t sum   = sigma + phi;

        /* Convert the sum to uint64_t for prime‑factor counting. */
        uint64_t sum64 = (uint64_t)sum;

        /* Count distinct prime factors of the sum. */
        if (count_distinct_primes(sum64) == 2)
        {
            /* Store this k into the cache. */
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)last_k;
        }
    }

    /* Cache is 0‑based; convert to 1‑based index. */
    return cache[n - 1];
}
