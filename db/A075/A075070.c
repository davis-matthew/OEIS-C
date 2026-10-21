/*
 * A075070  a(n) = n-th compositorial number / (product of those primes which
 *          divide the n-th compositorial number).
 * https://oeis.org/A075070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A036691(n) / rad(A036691(n)), i.e. product of first n
 *            composite numbers divided by the product of distinct primes that
 *            divide that product
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define LIMIT 2000000   /* large enough to cover composites up to n ~ 10000 */

seq_t a(idx_t n)
{
    /* ---------- static data ------------------------------------ */
    static bool initialized = false;
    static uint32_t spf[LIMIT + 1];
    static bool prime_seen[LIMIT + 1];
    static seq_t *cache = NULL;      /* cached a(0..cache_size-1) */
    static size_t cache_size = 0;    /* number of cached terms */
    static size_t cache_cap  = 0;    /* capacity of cache */
    static uint32_t last_checked = 3;/* last number examined for composites */
    static seq_t current_val = (seq_t)1; /* a(current_index) */

    /* ---------- allocate initial cache ------------------------ */
    if (!cache_cap) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    /* ---------- ensure cache has enough capacity ------------- */
    if (cache_size + 1 > cache_cap) {
        cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
    }

    /* ---------- initialization -------------------------------- */
    if (!initialized) {
        /* fill SPF array */
        for (uint32_t i = 0; i <= LIMIT; ++i) {
            spf[i] = i;
        }
        for (uint32_t i = 2; i * i <= LIMIT; ++i) {
            if (spf[i] == i) {                 /* i is prime */
                for (uint32_t j = i * i; j <= LIMIT; j += i) {
                    if (spf[j] == j) spf[j] = i;
                }
            }
        }
        /* initialize prime_seen flags */
        for (uint32_t i = 0; i <= LIMIT; ++i) {
            prime_seen[i] = false;
        }
        /* cache a(0) */
        cache[0] = current_val;
        cache_size = 1;
        initialized = true;
    }

    /* ---------- extend cache up to index n -------------------- */
    while (cache_size <= n) {
        /* find next composite number */
        uint32_t comp;
        do {
            ++last_checked;
        } while (last_checked <= LIMIT && spf[last_checked] == last_checked);
        comp = last_checked;

        /* multiply current value by this composite */
        current_val *= (seq_t)comp;

        /* factor out the composite, dividing by each new prime once */
        uint32_t x = comp;
        while (x > 1) {
            uint32_t p = spf[x];
            /* skip duplicated prime factors to get distinct primes */
            while (x % p == 0) {
                x /= p;
            }
            if (!prime_seen[p]) {
                current_val /= (seq_t)p;
                prime_seen[p] = true;
            }
        }

        /* store the new term, resize cache if needed */
        if (cache_size >= cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        }
        cache[cache_size] = current_val;
        ++cache_size;
    }

    return cache[n];
}
