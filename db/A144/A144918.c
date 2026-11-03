/*
 * A144918  Duplicate of A007528.
 * https://oeis.org/A144918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) is the n‑th prime congruent to 5 modulo 6 (primes of the
 *            form 6k-1)
 * Range:     i8 n <= 15, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The value is computed by searching primes of that form and
   testing each candidate with the provided is_prime_big routine. */

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

static seq_t *cache      = NULL;   /* cached terms */
static size_t cache_cap  = 0;      /* cache capacity */
static size_t cache_len  = 0;      /* number of cached terms */
static seq_t last_candidate = 0;   /* last generated 6k-1 prime */

/* Ensure that at least n terms are cached. */
static void ensure_terms(idx_t n)
{
    if (n <= cache_len)
        return;

    /* Generate terms until we have at least n. */
    while (cache_len < n) {
        seq_t cand;
        if (cache_len == 0) {
            cand = 5;                      /* first 6k-1 prime */
        } else {
            cand = last_candidate + (seq_t)6;
        }

        /* Skip non‑primes; only numbers of the form 6k-1 are tested. */
        while (!is_prime_big(cand))
            cand += (seq_t)6;

        /* Grow the cache if necessary. */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 32;
            seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
            if (!new_cache)          /* allocation failure – abort */
                return;
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Store the found prime. */
        cache[cache_len++] = cand;
        last_candidate = cand;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)          /* not defined for n=0 – return 0 */
        return 0;
    ensure_terms(n);
    return cache[n - 1];
}
