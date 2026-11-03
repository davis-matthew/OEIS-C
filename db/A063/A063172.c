/*
 * A063172  Composite numbers which in base 9 contain their largest proper
 *          factor as a substring.
 * https://oeis.org/A063172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-01)
 * Helpers:   smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A063172: a(n) is the nth composite integer whose base‑9
   representation contains its largest proper factor (written in
   decimal) as a substring.  The function searches for the next
   qualifying integer and memoises results in a static cache. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Helper functions provided by the build */
#include "../utils/smallest_prime_factor.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Convert a number to its base‑9 string representation.
   buf must be large enough (64 chars is ample). */
static void to_base9(seq_t x, char *buf)
{
    if (x == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    char rev[64];
    int pos = 0;
    while (x != 0) {
        int d = (int)(x % 9);
        rev[pos++] = '0' + d;
        x /= 9;
    }
    /* reverse */
    for (int i = 0; i < pos / 2; ++i) {
        char t = rev[i];
        rev[i] = rev[pos - 1 - i];
        rev[pos - 1 - i] = t;
    }
    memcpy(buf, rev, pos);
    buf[pos] = '\0';
}

static seq_t *cache = NULL;        /* cache of found terms   */
static size_t cache_size = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* capacity of cache     */
static uint64_t next_candidate = 2;/* next integer to test  */

/* Main sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined index, safe fallback */

    /* Generate terms until we reach the requested index */
    while (cache_size < (size_t)n) {
        uint64_t cand = next_candidate++;   /* candidate number */

        /* Skip primes */
        uint64_t spf = smallest_prime_factor(cand);
        if (spf == cand) continue;           /* candidate is prime */

        /* Largest proper factor */
        uint64_t largest = cand / spf;

        /* Base‑9 strings */
        char cand_str[64];
        char largest_str[64];
        to_base9((seq_t)cand, cand_str);
        to_base9((seq_t)largest, largest_str);

        /* Check substring: largest factor written in decimal */
        if (strstr(cand_str, largest_str) != NULL) {
            /* Ensure capacity */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)cand;
        }
    }

    return cache[n - 1];
}
