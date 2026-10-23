/*
 * A132936  Concatenation of first n isolated primes.
 * https://oeis.org/A132936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = concatenate the first n isolated primes * An isolated
 *            prime is a prime p such that neither p−2 nor p+2 is prime. * The
 *            function builds the list of isolated primes lazily and caches *
 *            the concatenated results so that a(n) can be returned in O(1)
 *            after * the required primes have been found
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   digit_len, is_prime_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Utility functions supplied by the build */
#include "../utils/is_prime_small.h"  /* bool is_prime_small(seq_t n); */
#include "../utils/digit_len.h"       /* int digit_len(seq_t x); */
#include "../utils/pow10.h"           /* seq_t pow10(idx_t e); */

static uint64_t *isolated_primes = NULL;   /* array of isolated prime values */
static size_t isolated_cap   = 0;
static size_t isolated_cnt   = 0;
static uint64_t last_checked = 1;          /* last number examined for isolation */

/* Cache of concatenated values */
static seq_t *value_cache = NULL;   /* array of a(k) */
static size_t cache_cap   = 0;
static size_t cache_cnt   = 0;

/* Ensure we have at least 'need' isolated primes stored. */
static void ensure_isolated(idx_t need)
{
    while (isolated_cnt < need) {
        /* Advance to next candidate prime */
        last_checked += 1;

        /* Handle 2 separately */
        if (last_checked == 2) {
            if (is_prime_small((seq_t)2)) {
                if (isolated_cnt >= isolated_cap) {
                    isolated_cap = isolated_cap ? isolated_cap * 2 : 16;
                    isolated_primes = (uint64_t*)realloc(isolated_primes,
                        isolated_cap * sizeof(uint64_t));
                }
                isolated_primes[isolated_cnt++] = 2;
            }
            continue;
        }

        /* Skip even numbers */
        if ((last_checked & 1u) == 0) continue;

        uint64_t p = last_checked;
        if (!is_prime_small((seq_t)p)) continue;

        bool iso = true;
        if (p > 2 && is_prime_small((seq_t)(p - 2))) iso = false;
        if (is_prime_small((seq_t)(p + 2))) iso = false;

        if (iso) {
            if (isolated_cnt >= isolated_cap) {
                isolated_cap = isolated_cap ? isolated_cap * 2 : 16;
                isolated_primes = (uint64_t*)realloc(isolated_primes,
                    isolated_cap * sizeof(uint64_t));
            }
            isolated_primes[isolated_cnt++] = p;
        }
    }
}

/* Ensure the cached concatenated values have at least 'need' entries. */
static void ensure_cache(idx_t need)
{
    while (cache_cnt < need) {
        uint64_t p = isolated_primes[cache_cnt];
        int d   = digit_len((seq_t)p);          /* number of decimal digits of p */
        seq_t pow = pow10((idx_t)d);            /* 10^d */
        seq_t res = (cache_cnt == 0) ? 0
                                   : value_cache[cache_cnt - 1];
        res = res * pow + (seq_t)p;

        if (cache_cnt >= cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 16;
            value_cache = (seq_t*)realloc(value_cache,
                cache_cap * sizeof(seq_t));
        }
        value_cache[cache_cnt++] = res;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, harmless */

    ensure_isolated(n);
    ensure_cache(n);

    return value_cache[n - 1];
}
