/*
 * A224781  Primes p such that both 2*p + 1 and p^2 + p + 1 are primes.
 * https://oeis.org/A224781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 7, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A224781: Primes p such that 2p+1 and p^2+p+1 are primes.
 * This implementation lazily generates terms and caches them for
 * fast replay. It works for any seq_t/idx_t combination supported by
 * the build (including 8‑, 16‑, 32‑, 64‑, 128‑bit unsigned or
 * arbitrary-precision types).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"      /* bool is_prime(idx_t n) */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n) */

/* --- Static state for memoisation ------------------------------------ */
static seq_t *cache    = NULL;   /* array holding found primes */
static size_t cache_len = 0;      /* number of terms stored */
static size_t cache_cap = 0;      /* allocated capacity */
static idx_t  last_checked = 0;   /* largest integer already examined */

/* Ensure that the cache can hold at least ‘needed’ elements. */
static void ensure_cache_capacity(size_t needed) {
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure – terminate */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Append a newly discovered term to the cache. */
static void append_term(seq_t val) {
    ensure_cache_capacity(cache_len + 1);
    cache[cache_len++] = val;
}

/* --- Required sequence function ------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;      /* undefined for offset 1, but harmless */

    /* Grow the cache until it holds at least n terms. */
    while (cache_len < (size_t)n) {
        /* Start searching from the next integer after the last checked one. */
        idx_t candidate = last_checked + 1;

        /* Ensure candidate is at least 2 and odd (except for 2 itself). */
        if (candidate <= 2) {
            candidate = 2;
        } else if ((candidate & 1) == 0) {
            candidate++;
        }

        /* Scan subsequent odd numbers until we find another term. */
        for (;;) {
            last_checked = candidate; /* mark this number as examined */

            /* Test that candidate itself is prime. */
            if (is_prime(candidate)) {
                seq_t two_p_plus_one     = (seq_t)candidate * 2 + 1;
                seq_t p_sq_plus_p_plus_one = (seq_t)candidate * candidate
                                             + (seq_t)candidate + 1;

                if (is_prime_big(two_p_plus_one) &&
                    is_prime_big(p_sq_plus_p_plus_one)) {
                    append_term(candidate);
                    if (cache_len >= (size_t)n) break;
                }
            }

            /* Advance to the next possible prime candidate. */
            if (candidate == 2) {
                candidate = 3;
            } else {
                candidate += 2;            /* skip even numbers */
            }
        }
    }

    return cache[n - 1];
}
