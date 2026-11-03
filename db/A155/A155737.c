/*
 * A155737  Primes of the form 4*n^2 + 2*n -1.
 * https://oeis.org/A155737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 57, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 880 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A155737 – primes of the form 4*n^2 + 2*n – 1.
   The n‑th term is the n‑th prime that can be expressed by this
   quadratic.  Candidates are generated sequentially and tested with
   is_prime_big().  Results are cached so that repeated calls to any
   n are fast. */
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

static seq_t *cache = NULL;      /* array of found primes          */
static size_t cache_count = 0;   /* number of primes stored        */
static size_t cache_cap   = 0;   /* capacity of the array          */
static idx_t  last_k      = 0;   /* last candidate k processed     */

/* Grow the cache array to at least new_cap elements.
   aborts on allocation failure by resetting the cache. */
static void
grow_cache(size_t new_cap)
{
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) {
        mem_free(cache);
        cache = NULL;
        cache_count = 0;
        cache_cap   = 0;
        last_k      = 0;
        return;
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t
a(idx_t n)
{
    /* Note: OEIS offset is 1; a(1) is the first prime of this form. */
    if (n == 0u)
        return 0;                     /* undefined for non‑positive indices */

    /* If the value is already cached, return it. */
    if ((size_t)n <= cache_count)
        return cache[(size_t)n - 1];

    /* Ensure sufficient capacity for the desired index. */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < (size_t)n)
            new_cap = (size_t)n;
        grow_cache(new_cap);
        if (cache_cap == 0)          /* allocation failed */
            return 0;
    }

    /* Generate candidates until the n‑th prime of this form is found. */
    for (idx_t k = last_k + 1; ; ++k) {
        seq_t k_seq = (seq_t)k;
        seq_t val   = (4 * k_seq * k_seq) + (2 * k_seq) - 1;

        if (is_prime_big(val)) {
            cache[cache_count++] = val;

            if ((size_t)cache_count == n) {
                last_k = k;          /* remember last tested k          */
                return val;
            }
        }
    }
}
