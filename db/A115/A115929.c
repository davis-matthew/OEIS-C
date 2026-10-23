/*
 * A115929  Numbers k such that k and 4*k, taken together, are zeroless
 *          pandigital, that is, use all the digits 1-9 exactly once.
 * https://oeis.org/A115929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th integer k such that the 4 digits of k and the
   5 digits of 4*k together contain each of the digits 1–9 exactly once. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_k_tested = 2499;   /* start search at 2500 */

/* Append value to cache, reallocating if needed. */
static void append_to_cache(seq_t val)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_len == cache_cap) {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

/* Return mask of digits 1–9 used in number n, or 0 if n contains
   a 0 or repeated digits. */
static uint16_t digit_mask(uint64_t n)
{
    uint16_t mask = 0;
    while (n > 0) {
        uint8_t d = n % 10;
        if (d == 0)
            return 0;
        uint16_t bit = 1u << (d - 1);
        if (mask & bit)
            return 0;                 /* repeated digit */
        mask |= bit;
        n /= 10;
    }
    return mask;
}

/* Search for next term if needed, until cache contains at least n terms. */
static void ensure_terms(idx_t n)
{
    while ((idx_t)cache_len < n) {
        if (last_k_tested >= 9999)
            break;                    /* no more candidates */
        ++last_k_tested;
        /* k must be 4‑digit and 4*k must be 5‑digit. */
        if (last_k_tested < 2500)
            continue;

        uint64_t k = (uint64_t)last_k_tested;
        uint64_t four = k * 4ULL;

        if (four >= 100000ULL)        /* must be exactly 5 digits */
            continue;

        uint16_t mk = digit_mask(k);
        if (mk == 0)
            continue;

        uint16_t mf = digit_mask(four);
        if (mf == 0)
            continue;

        /* No overlap between the two masks, and together they use all 9 digits. */
        if ( (mk & mf) != 0 )
            continue;
        if ( (mk | mf) != 0x1FFu )    /* 511 decimal = bits 0..8 set */
            continue;

        append_to_cache((seq_t)k);
    }
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;               /* undefined for n<1 */
    ensure_terms(n);
    if ((idx_t)cache_len < n)
        return (seq_t)0;               /* not enough terms found */
    return cache[n - 1];
}
