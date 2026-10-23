/*
 * A156981  Numbers n with property that n^2 and (n+1)^2 have no common
 *          digits.
 * https://oeis.org/A156981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based implementation of A156981: numbers n such that n² and (n+1)² have no common decimal digits. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Static cache for computed sequence terms (a(1), a(2), …). */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Next integer candidate to test (0‑based). */
static idx_t next_candidate = 0;

/* Helper: test whether two seq_t numbers have no common decimal digits. */
static inline bool no_common_digits(seq_t a, seq_t b)
{
    uint16_t mask_a = 0, mask_b = 0;

    /* digits of a */
    seq_t tmp = a;
    if (tmp == (seq_t)0) {
        mask_a |= 1u << 0;
    } else {
        while (tmp != (seq_t)0) {
            int d = (int)(tmp % (seq_t)10);
            mask_a |= 1u << d;
            tmp /= (seq_t)10;
        }
    }

    /* digits of b */
    tmp = b;
    if (tmp == (seq_t)0) {
        mask_b |= 1u << 0;
    } else {
        while (tmp != (seq_t)0) {
            int d = (int)(tmp % (seq_t)10);
            mask_b |= 1u << d;
            tmp /= (seq_t)10;
        }
    }

    return (mask_a & mask_b) == 0;
}

/* Append a value to the static cache, growing it if necessary. */
static void append_to_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
        /* Cast needed for C++ compilation when seq_t is a class type. */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for offset 1; return 0 as a safe placeholder. */
        return (seq_t)0;
    }

    /* Grow cache until it contains a(n). */
    while (cache_len < n) {
        seq_t cand   = (seq_t)next_candidate;             /* current n to test */
        seq_t sq1    = cand * cand;                       /* n² */
        seq_t sq2    = (cand + (seq_t)1) * (cand + (seq_t)1); /* (n+1)² */

        if (no_common_digits(sq1, sq2)) {
            append_to_cache(cand); /* store good number */
        }

        ++next_candidate; /* advance to next integer */
    }

    return cache[n - 1]; /* cache is 0‑based, sequence offset is 1 */
}
