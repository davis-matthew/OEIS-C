/*
 * A119409  Numbers k such that 235*k + 1 is prime.
 * https://oeis.org/A119409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = the n-th integer k such that 235*k + 1 is prime
 * Range:     u8 n <= 1, u16 n <= 33, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

seq_t a(idx_t n)
{
    /* Cache of found k values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    /* Next integer to test for the property */
    static idx_t next_candidate = 1;

    if (n == 0) {
        /* Offset is 1; return 0 in a graceful fallback */
        return (seq_t)0;
    }

    /* Return cached value if already known */
    if (n <= cache_size) {
        return cache[(size_t)(n - 1)];
    }

    /* Initial allocation of the cache if necessary */
    if (cache_cap == 0) {
        cache_cap = 32;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) {
            abort(); /* allocation failure */
        }
    }

    /* Generate k values until we have at least n in the cache */
    while (cache_size < n) {
        seq_t k = (seq_t)next_candidate++;
        seq_t m = (seq_t)235 * k + (seq_t)1;

        if (is_prime_big(m)) {
            /* Ensure enough space in the cache */
            if (cache_size == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                if (!tmp) {
                    mem_free(cache);
                    abort();
                }
                cache = tmp;
            }
            cache[cache_size++] = k;
        }
    }

    return cache[(size_t)(n - 1)];
}
