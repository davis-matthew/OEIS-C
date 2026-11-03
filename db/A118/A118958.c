/*
 * A118958  Primes that cannot be written as 2^k + p with p prime < 2^k.
 * https://oeis.org/A118958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 5489, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9659 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A118958 – primes that cannot be written as 2^k + p with
    p prime < 2^k.
    The implementation searches successive primes and tests the
    defining property.  All found terms are stored in a static cache
    so that any later call with the same or a larger index can
    answer in O(1).  The algorithm is valid for both C11 and C++17.
    It uses the utility function is_prime_big(seq_t) for primality.
*/

#include <stddef.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache_vec;
static seq_t last_checked = 1;
#else
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t last_checked = 1;
#endif

/* add a new term to the cache */
static void add_to_cache(seq_t val)
{
#ifdef __cplusplus
    cache_vec.push_back(val);
#else
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;               /* allocation failed – abort */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
#endif
}

/* current cache size */
static size_t cache_size(void)
{
#ifdef __cplusplus
    return cache_vec.size();
#else
    return cache_len;
#endif
}

/* retrieve element at zero‑based index */
static seq_t cache_get(size_t idx)
{
#ifdef __cplusplus
    return cache_vec[idx];
#else
    return cache[idx];
#endif
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    /* Generate terms until we have enough */
    while (cache_size() < n) {
        /* move to the next candidate prime */
        do
            last_checked++;
        while (last_checked > 2 && (last_checked & 1) == 0);      /* skip evens >2 */

        if (!is_prime_big(last_checked))
            continue;                                       /* not a prime */

        /* check the defining property */
        bool ok = true;
        seq_t pow = (seq_t)2;
        while (pow < last_checked) {
            seq_t rem = last_checked - pow;
            if (rem > 0 && is_prime_big(rem) && rem < pow) {
                ok = false;                                /* decomposition found */
                break;
            }
            /* avoid overflow when doubling pow */
            if (pow > last_checked / 2)
                break;
            pow += pow;   /* next power of two */
        }

        if (ok)                 /* success – store the prime */
            add_to_cache(last_checked);
    }

    return cache_get(n - 1);       /* zero‑based indexing */
}
