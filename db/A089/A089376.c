/*
 * A089376  Primes of the form k^2 - 7*k + 7.
 * https://oeis.org/A089376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 51, u32 n <= 1, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 56 terms through n = 4862 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = value of the n-th prime of the form k^2 - 7*k + 7,
 * computed by incremental search.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
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

static seq_t *cache = NULL;      /* array of found primes */
static size_t cache_cap = 0;     /* allocated capacity of cache   */
static size_t cache_len = 0;     /* number of primes stored       */
static idx_t next_k = 0;         /* next k to test                */

/* Ensure cache has at least `needed` slots */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 128;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp)
        return; /* allocation failure – leave cache unchanged */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th prime of the form k^2 - 7*k + 7 (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0)           /* undefined; caller should not use */
        return (seq_t)0;

    /* lazy initialise the cache with the first occurrence (k = 0) */
    if (cache_len == 0) {
        ensure_cache_cap(1);
        cache[0] = (seq_t)7;    /* k = 0 gives 7 */
        cache_len = 1;
        next_k   = 1;           /* start testing k = 1 */
    }

    /* generate primes until we have at least n of them */
    while (cache_len < (size_t)n) {
        seq_t k   = (seq_t)next_k;
        seq_t val = k * k - (seq_t)7 * k + (seq_t)7;

        if (val >= (seq_t)2 && is_prime_big(val)) {
            /* avoid duplicate 7 at k = 7 */
            if (cache_len == 0 || val != cache[cache_len - 1]) {
                ensure_cache_cap(cache_len + 1);
                cache[cache_len++] = val;
            }
        }
        next_k++;
    }
    return cache[n - 1];
}
