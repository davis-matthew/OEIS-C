/*
 * A073945  Numbers n such that n + pi(n) is a square.
 * https://oeis.org/A073945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer m ≥ 0 such that m + π(m) is a perfect
 *            square
 * Range:     u8 n <= 14, u16 n <= 240, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Cached sequence values (stored as the full integer type) and auxiliary data */
static idx_t *cached_vals = NULL;     /* array of m's that satisfy the condition */
static size_t cache_len = 0;
static size_t cache_cap = 0;

static idx_t *prime_list = NULL;      /* list of primes found so far */
static size_t primes_len = 0;
static size_t primes_cap = 0;

static idx_t known_limit = 1;          /* largest integer examined (exclusive) */
static idx_t prime_count = 0;           /* π(known_limit) */

/* ------------------------------------------------------------------ */
/* Utility: floor(sqrt(x)) for idx_t (works for 0 … 2^64-1)           */
static idx_t isqrt_idx(idx_t x)
{
    idx_t lo = 0, hi = x, mid, ans = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        if (mid <= x / mid) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;   /* avoid infinite loop when x=0 */
            hi = mid - 1;
        }
    }
    return ans;
}

/* ------------------------------------------------------------------ */
/* Ensure cache has capacity for at least one more element */
static void ensure_cache_cap(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        idx_t *tmp = (idx_t *)realloc(cached_vals, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        cached_vals = tmp;
        cache_cap = new_cap;
    }
}

/* Ensure prime list has capacity for at least one more prime */
static void ensure_prime_cap(void)
{
    if (primes_len >= primes_cap) {
        size_t new_cap = primes_cap ? primes_cap << 1 : 16;
        idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        prime_list = tmp;
        primes_cap = new_cap;
    }
}

/* Simple primality test using the current prime list (trial division) */
static int is_prime_local(idx_t n)
{
    if (n < 2) return 0;
    idx_t lim = isqrt_idx(n);
    for (size_t i = 0; i < primes_len; ++i) {
        idx_t p = prime_list[i];
        if (p > lim) break;
        if (n % p == 0) return 0;
    }
    return 1;
}

/* ------------------------------------------------------------------ */
/* Main function requested by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for the OEIS offset, but harmless */

    /* Initialize on first call: pre-fill cache with 0 and 1 */
    if (cache_len == 0) {
        ensure_cache_cap();
        cached_vals[cache_len++] = 0;   /* a(1) = 0 */
        ensure_cache_cap();
        cached_vals[cache_len++] = 1;   /* a(2) = 1 */
    }

    /* Generate values until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* Advance to next integer */
        ++known_limit;

        /* Update prime list and count if the new number is prime */
        if (is_prime_local(known_limit)) {
            ensure_prime_cap();
            prime_list[primes_len++] = known_limit;
            ++prime_count;
        }

        /* Check whether known_limit + π(known_limit) is a perfect square */
        idx_t sum = known_limit + prime_count;
        idx_t r   = isqrt_idx(sum);
        if (r * r == sum) {
            ensure_cache_cap();
            cached_vals[cache_len++] = known_limit;
        }
    }

    /* Return the requested term (converted to the build's seq_t type) */
    return (seq_t)cached_vals[n - 1];
}
