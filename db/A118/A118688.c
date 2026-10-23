/*
 * A118688  Semiprimes for which the sum of the digits is also a semiprime.
 * https://oeis.org/A118688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 5779, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: semiprimes whose digit sum is also a semiprime (OEIS A118688) */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* helper: prime test (trial division) */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t i = 3; i * i <= n; i += 2)
        if (n % i == 0) return false;
    return true;
}

/* helper: exact digit sum */
static uint64_t digit_sum_uint64(uint64_t x)
{
    uint64_t sum = 0;
    while (x) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

/* helper: semiprime test (exactly two prime factors, counting multiplicity) */
static bool is_semiprime_uint64(uint64_t n)
{
    if (n < 4) return false;                /* 1,2,3 are not semiprimes */
    uint64_t p = 0;
    /* find smallest prime divisor */
    if (n % 2 == 0) {
        p = 2;
    } else {
        for (uint64_t i = 3; i * i <= n; i += 2) {
            if (n % i == 0) {
                p = i;
                break;
            }
        }
    }
    if (p == 0) return false;               /* n is prime */
    uint64_t q = n / p;
    return is_prime_uint64(q);              /* second factor must be prime */
}

/* static cache for memoization */
static seq_t *cache = NULL;
static size_t cache_len  = 0;   /* number of elements stored */
static size_t cache_cap  = 0;   /* capacity of the cache array */

/* next integer to test (starts at first candidate > 0) */
static uint64_t next_candidate = 4;   /* smallest semiprime */

/* ensure cache has at least 'need' terms */
static void ensure_cache(idx_t need)
{
    while (cache_len < need) {
        /* find next candidate satisfying both conditions */
        while (true) {
            if (is_semiprime_uint64(next_candidate)) {
                uint64_t ds = digit_sum_uint64(next_candidate);
                if (is_semiprime_uint64(ds)) break;
            }
            ++next_candidate;
        }

        /* grow cache array if needed */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* store the found term */
        cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n=0 or negative */
    ensure_cache(n);
    return cache[n-1];
}
