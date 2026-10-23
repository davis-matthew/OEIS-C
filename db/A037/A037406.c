/*
 * A037406  Numbers k such that every base-8 digit of k is a base-10 digit of
 *          k.
 * https://oeis.org/A037406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1016, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037406: numbers k such that every base‑8 digit of k is a base‑10
 * digit of k.  The function returns the n‑th term (n ≥ 1).  It uses a
 * lazy, incremental cache so that repeated calls in any order are
 * efficient.  All arithmetic is performed with the allowed operators.
 */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Check whether k satisfies the required property. */
static bool has_property(idx_t k)
{
    /* build mask of decimal digits of k */
    unsigned int decMask = 0;
    idx_t tmp = k;
    while (tmp > 0) {
        unsigned int d = (unsigned int)(tmp % 10);
        decMask |= 1U << d;
        tmp /= 10;
    }

    /* verify each octal digit of k appears in the decimal mask */
    tmp = k;
    while (tmp > 0) {
        unsigned int od = (unsigned int)(tmp % 8);
        if ((decMask & (1U << od)) == 0)
            return false;
        tmp /= 8;
    }
    return true;
}

/* Incremental cache of sequence values */
static seq_t *cache = NULL;      /* array storing terms a(1), a(2), ... */
static size_t cache_len = 0;     /* number of terms stored (cache_len ≥ 1 when first term stored) */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t last_k = 0;         /* last integer examined for property */

/* Grow the cache to at least new_cap entries */
static void grow_cache(size_t new_cap)
{
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* out of memory – abort in a way that satisfies the build */
        abort();
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n = 0; return 0 as a safe fallback. */
        return (seq_t)0;
    }

    /* Ensure we have computed at least n terms. */
    while (cache_len < (size_t)n) {
        last_k++;                          /* candidate integer */
        if (has_property(last_k)) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 64;
                grow_cache(new_cap);
            }
            cache[cache_len] = (seq_t)last_k;
            ++cache_len;
        }
    }
    /* cache_len ≥ n, cache[0] holds a(1) */
    return cache[(size_t)n - 1];
}
