/*
 * A043229  Numbers k such that 1 and 6 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer k such that the decimal representation of
 *            k contains the substring "16" or "61" but k-1 does not
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_16_or_61
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/has_16_or_61.h"

/* Cached values of the sequence (1‑indexed). */
static seq_t *cache      = NULL;  /* array of sequence terms */
static size_t cache_cap  = 0;      /* allocated capacity of cache      */
static size_t cache_len  = 0;      /* number of terms stored           */
static seq_t  last_k     = 0;      /* highest integer examined so far   */

/* Grow the cache if necessary. */
static void cache_grow(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute the n‑th term of the sequence, with 1‑based indexing. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* n = 0 is outside the sequence range */
    while (cache_len < (size_t)n) {            /* generate terms until we have n of them */
        /* Find the next integer that contains "16" or "61". */
        do {
            ++last_k;
        } while (!has_16_or_61(last_k));

        /* Include it only if (k−1) does not contain the pattern. */
        if (!has_16_or_61(last_k - 1)) {
            cache_grow(cache_len + 1);
            cache[cache_len++] = last_k;
        }
    }
    return cache[n - 1];                       /* cache is 0‑indexed, n is 1‑based */
}
