/*
 * A094559  Number of words of length n over an alphabet of size 4 that are
 *          not "bifix-free".
 * https://oeis.org/A094559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the number of length‑n words over a 4‑letter alphabet
 *            that are not bifix‑free. Recurrence: a(0) = 0 a(1) = 0 a(2n) =
 *            4^n + 4*a(2n-1) - a(n) a(2n+1) = 4 * a(2n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation caches computed values and pre‑computes powers
   of 4 as needed.  It works for any unsigned integer type provided
   by the build (including arbitrary‑precision classes).  The code is
   valid C11 and C++17. */

#include <stdlib.h>
#include <stdbool.h>

/* static tables for caching */
static seq_t *cache   = NULL;   /* a(0)..a(n) */
static size_t cache_size = 0;   /* number of stored entries */

static seq_t *pow4    = NULL;   /* powers of 4: pow4[k] = 4^k */
static size_t pow4_size = 0;    /* number of stored powers */

/* Grow the table of 4^k up to exponent k (inclusive). */
static void ensure_pow4(size_t k)
{
    if (k < pow4_size) return;          /* already present */

    size_t new_size = k + 1;             /* need indices 0..k */
    pow4 = (seq_t *)realloc(pow4, new_size * sizeof(seq_t));
    if (!pow4) { pow4 = NULL; return; } /* allocation failure ignored */

    /* fill new entries from current size up to k */
    for (size_t i = pow4_size; i <= k; ++i) {
        if (i == 0) {
            pow4[0] = (seq_t)1;
        } else {
            pow4[i] = pow4[i - 1] * (seq_t)4;
        }
    }
    pow4_size = new_size;
}

/* Ensure that cache[0..n] are computed. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_size) return;  /* already computed */

    size_t new_size = (size_t)n + 1;
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!cache) { cache = NULL; return; } /* allocation failure ignored */

    /* initialise missing base cases if this is the first time */
    if (cache_size == 0) {
        cache[0] = (seq_t)0;
        if (new_size > 1) cache[1] = (seq_t)0;
    } else if (cache_size == 1) {
        cache[1] = (seq_t)0;
    }

    /* compute all new entries sequentially */
    for (size_t i = cache_size; i <= new_size - 1; ++i) {
        if (i == 0 || i == 1) {
            cache[i] = (seq_t)0;   /* base cases */
            continue;
        }
        if ((i & 1) == 0) {        /* even index: i = 2k */
            size_t k = i / 2;
            ensure_pow4(k);
            seq_t term = pow4[k];
            term += 4 * cache[i - 1];
            term -= cache[k];
            cache[i] = term;
        } else {                   /* odd index: i = 2k+1 */
            cache[i] = 4 * cache[i - 1];
        }
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
