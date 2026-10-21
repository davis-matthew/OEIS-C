/*
 * A067532  Numbers k such that k + number of divisors of k is a prime.
 * https://oeis.org/A067532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 5889, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A067532:  Numbers k such that k + d(k) is prime
   a(1) = 1, a(2) = 3, a(3) = 4, …  (offset 1) */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Helpers ----------------------------------------------------------------*/

static uint64_t divisor_count_u64(uint64_t n) {
    if (n <= 1) return 1;
    uint64_t count = 1;
    uint64_t m = n;
    uint64_t exp;

    /* factor 2 */
    exp = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        exp++;
    }
    if (exp) count *= (exp + 1);

    /* odd factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p != 0) continue;
        exp = 0;
        while (m % p == 0) {
            m /= p;
            exp++;
        }
        count *= (exp + 1);
    }
    if (m > 1) count *= 2;  /* remaining prime factor */
    return count;
}

static bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2ULL == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function ------------------------------------------------*/

seq_t a(idx_t n) {
    /* static cache of terms (stored as k) */
    static seq_t *cache = NULL;       /* array of seq_t */
    static size_t cache_len = 0;      /* number of valid elements */
    static size_t cache_cap = 0;      /* allocated capacity     */
    static uint64_t last_checked = 0;/* largest k examined so far */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0ULL;  /* out of memory */
    }

    /* grow cache until we have at least n terms */
    while (cache_len < n) {
        uint64_t k = last_checked + 1;
        last_checked = k;

        uint64_t d = divisor_count_u64(k);
        uint64_t sum = k + d;          /* k + d(k)  (fits in uint64_t) */
        if (is_prime_u64(sum)) {
            /* ensure capacity */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0ULL;  /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
