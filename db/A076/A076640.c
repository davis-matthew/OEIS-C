/*
 * A076640  a(1)=1, a(n) = a(n-phi(n)) + 1.
 * https://oeis.org/A076640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n - φ(n)) + 1, a(1) = 1 φ(n) is Euler’s totient
 *            function
 * Range:     u8 n <= 514, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We compute φ(n) on the fly with a simple trial‑division algorithm
   (sufficient for n up to ~10⁴, as required). */

#include <stdlib.h>

static seq_t *a_cache = NULL;   /* 1‑based cache, index 0 unused      */
static idx_t   cache_size = 0;   /* largest n that has been computed  */
static idx_t   cache_cap  = 0;   /* capacity of the allocated array   */

/* Ensure the cache can hold indices up to `need` (inclusive). */
static void grow_cache(idx_t need)
{
    if (need <= cache_cap) return;
    idx_t newcap = cache_cap ? (cache_cap << 1) : 8;
    while (newcap < need) newcap <<= 1;
    a_cache = (seq_t *)realloc(a_cache, newcap * sizeof(seq_t));
    cache_cap = newcap;
}

/* Euler’s totient function φ(n), n ≥ 1.  Result fits in seq_t. */
static seq_t phi(idx_t n)
{
    seq_t result = (seq_t)n;
    idx_t temp   = n;
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            while (temp % p == 0) temp /= p;
            result = result / (seq_t)p * (seq_t)(p - 1);
        }
    }
    if (temp > 1)
        result = result / (seq_t)temp * (seq_t)(temp - 1);
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety, sequence is defined for n ≥ 1 */

    /* Grow the cache to include index n. */
    grow_cache(n + 1);

    /* Initialise first term if not already done. */
    if (cache_size == 0)
    {
        a_cache[1] = 1;
        cache_size = 1;
    }

    /* Compute any missing values up to n. */
    for (idx_t i = cache_size + 1; i <= n; ++i)
    {
        seq_t phi_i   = phi(i);          /* φ(i) as seq_t */
        idx_t prev   = i - (idx_t)phi_i; /* previous index, guaranteed ≥ 1 */
        a_cache[i] = a_cache[prev] + 1;  /* recurrence */
    }

    cache_size = n;
    return a_cache[n];
}
