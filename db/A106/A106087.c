/*
 * A106087  Primes p such that 7*p+4 and 4*p+7 are primes.
 * https://oeis.org/A106087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches for primes p such that 7*p+4 and 4*p+7 are primes.
   The function caches already found terms and returns the n‑th term
   (n≥1).  It generates terms on demand; the cache is maintained
   between calls. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

/* Helper for 128‑bit arithmetic if the compiler supports it. */
#if defined(__SIZEOF_INT128__) || defined(__INT128__)
typedef unsigned __int128 uint128_t;
#else
/* Fall back to 64‑bit when 128‑bit is unavailable.  In that case
   seq_t will never be larger than 64 bits, so the code still works. */
typedef unsigned long long uint128_t;
#endif

/* Dynamic cache of found terms */
static seq_t *cache     = NULL;
static idx_t cache_cnt  = 0;
static idx_t cache_cap  = 0;

/* Utility: grow the cache when needed. */
static void cache_add(seq_t val) {
    if (cache_cnt == cache_cap) {
        idx_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_cnt++] = val;
}

/* Compute (seq_t)-1 in a 128‑bit type, used to check overflow. */
static bool   max_inited      = false;
static uint128_t max_seq128   = 0;

/* Initialise max_seq128 once. */
static void init_max_seq(void) {
    if (max_inited) return;
    seq_t max_val = (seq_t)-1;
    max_seq128 = (uint128_t)max_val;
    max_inited = true;
}

/* Last prime candidate that was examined (as an odd number, 2 is handled separately). */
static unsigned long long last_checked = 0;   /* start before first candidate */

/* Produce the next prime candidate (2,3,5,7,9,...). */
static unsigned long long next_candidate(void) {
    if (last_checked < 2) {
        last_checked = 2;
        return 2;
    }
    if (last_checked == 2) {
        last_checked = 3;
        return 3;
    }
    last_checked += 2;            /* skip even numbers */
    return last_checked;
}

/* Main sequence routine. */
seq_t a(idx_t n) {
    if (n == 0) abort();           /* offset is 1 */

    init_max_seq();                  /* ensure max_seq128 is ready */

    /* Generate terms until we have at least n of them. */
    while (cache_cnt < n) {
        unsigned long long cand = next_candidate();
        seq_t p = (seq_t)cand;

        /* Candidate must be a prime itself. */
        if (is_prime_seq(p)) {
            /* Compute 7*p+4 and 4*p+7 using 128‑bit to avoid overflow. */
            uint128_t v1 = (uint128_t)7 * cand + 4;
            uint128_t v2 = (uint128_t)4 * cand + 7;

            /* If either value exceeds the representable range of seq_t,
               we cannot check its primality correctly, so stop searching. */
            if (v1 > max_seq128 || v2 > max_seq128) {
                break;
            }

            seq_t val1 = (seq_t)v1;
            seq_t val2 = (seq_t)v2;

            if (is_prime_seq(val1) && is_prime_seq(val2)) {
                cache_add(p);
            }
        }
    }

    /* If we ran out of terms, return 0 (should not happen for valid inputs
       that fit into seq_t). */
    return (cache_cnt >= n) ? cache[cache_cnt - 1] : (seq_t)0;
}
