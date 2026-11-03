/*
 * A053790  Composite numbers arising as sum of first k primes.
 * https://oeis.org/A053790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth composite sum of the first k primes. The function
 *            generates the sums incrementally and caches them, so repeated
 *            calls are O(1)
 * Range:     u8 n <= 8, u16 n <= 138, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9953 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include "../utils/next_prime.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

#ifdef __cplusplus
#endif

seq_t a(idx_t n)
{
    /* Static state: caching computed terms and maintaining the current
       running sum of primes. */
    static seq_t *cache      = NULL; /* cached composite sums   */
    static size_t cache_cap  = 0;    /* allocated size of cache */
    static idx_t  cached_n      = 0; /* number of cached terms   */
    static seq_t   current_sum   = 0; /* running sum of primes    */
    static uint64_t current_prime = 1;/* last prime used          */
    static bool initialized = false; /* initialization flag     */

    /* One‑time allocation of cache storage. */
    if (!initialized)
    {
        initialized = true;
        cache_cap = 16; /* initial modest capacity */

#ifdef __cplusplus
        cache = seq_alloc_new_array(cache_cap);
#else
        /* In a purely C build we must use malloc; cast to seq_t* is fine. */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
#endif
        if (!cache)
            abort(); /* ensure that we don't continue with a null pointer */
    }

    /* Return immediately if the requested term has already been cached. */
    if (n <= cached_n)
        return cache[n - 1];

    /* Generate additional composite sums until we reach the desired index. */
    while (cached_n < n)
    {
        /* Get the next prime after the current one. */
        current_prime = next_prime(current_prime);

        /* Update the running sum; conversion to seq_t is safe for all
           supported seq_t sizes. */
        current_sum += (seq_t)current_prime;

        /* If the sum is composite, add it to the cache. */
        if (!is_prime_big(current_sum))
        {
            ++cached_n; /* new term discovered */

            /* Grow the cache if necessary. */
            if (cached_n > cache_cap)
                grow_seq(&cache, &cache_cap, (size_t)cached_n);

            cache[cached_n - 1] = current_sum;
        }
    }

    return cache[n - 1];
}
