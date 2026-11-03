/*
 * A037424  Positive numbers having the same set of digits in base 4 and base
 *          6.
 * https://oeis.org/A037424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose set of digits in base 4
 *            equals its set of digits in base 6. The function builds the
 *            sequence lazily and reuses previously computed terms
 * Range:     u8 n <= 9, u16 n <= 1901, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9941 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Return true iff the set of digits of x in base 4 equals the set of digits
   of x in base 6 (and all digits seen in base 6 belong to {0,1,2,3}). */
static bool same_digit_set(seq_t x)
{
    unsigned mask4 = 0, mask6 = 0;
    seq_t t = x;

    /* base 4 digits */
    do {
        seq_t d = t % (seq_t)4;
        mask4 |= 1u << (unsigned)d;
        t /= (seq_t)4;
    } while (t);

    t = x;
    /* base 6 digits (reject if >3) */
    do {
        seq_t d = t % (seq_t)6;
        if (d > (seq_t)3) return false;
        mask6 |= 1u << (unsigned)d;
        t /= (seq_t)6;
    } while (t);

    return mask4 == mask6;
}

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Lazily extend cache until it contains at least n terms. */
static void ensure_terms(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    seq_t cand = cache_len ? cache[cache_len-1] + 1 : 1;

    while (cache_len < (size_t)n) {
        if (same_digit_set(cand))
            /* grow cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort(); /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
        if (same_digit_set(cand)) {
            cache[cache_len++] = cand;
            /* if already added, increment next candidate after storing */
            cand++;
            continue;
        }
        cand++;
    }
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return 0;   /* out of range */
    ensure_terms(n);
    return cache[(size_t)n - 1];
}
