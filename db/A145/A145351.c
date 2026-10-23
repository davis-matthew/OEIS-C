/*
 * A145351  Prime-indexed composites k such that lpf(k) + gpf(k) is a prime.
 * https://oeis.org/A145351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 794, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gpf, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the n-th composite number whose composite index
 * (i.e., its rank among composites) is a prime and for which
 * lpf(k)+gpf(k) is also prime. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/gpf.h"

/* small helper: smallest prime factor of a uint64_t (composite only). */
static inline uint64_t lpf_uint64(uint64_t n)
{
    for (uint64_t p = 2; p * p <= n; ++p)
        if (n % p == 0)
            return p;
    return n;   /* should not happen for composites */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n = 0 */

    /* ---------- cache handling ---------- */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of terms stored */
    static size_t cache_cap = 0;  /* allocated capacity */

    if (n <= cache_len)
        return cache[n - 1];

    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap == 0 ? n : cache_cap * 2;
        if (new_cap < (size_t)n)
            new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* ---------- state that persists across calls ---------- */
    static uint64_t last_checked = 4ULL;    /* next integer to test */
    static size_t comp_idx = 0;             /* number of composites seen */
    static bool init = false;

    if (!init) {
        last_checked = 4ULL;
        comp_idx = 0;
        init = true;
    }

    /* ---------- search until we have the required term ---------- */
    while (cache_len < (size_t)n) {
        uint64_t num = last_checked++;
        if (is_prime(num))
            continue;                       /* skip primes */

        /* composite found */
        ++comp_idx;

        /* composite index must be prime itself */
        if (!is_prime((idx_t)comp_idx))
            continue;

        uint64_t l = lpf_uint64(num);
        uint64_t g = gpf(num);
        uint64_t sum = l + g;

        if (is_prime(sum))
            cache[cache_len++] = (seq_t)num;
    }

    return cache[n - 1];
}
