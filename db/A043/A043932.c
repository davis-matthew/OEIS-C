/*
 * A043932  Numbers k such that 0 and 1 occur juxtaposed in the base-8
 *          representation of k but not of k+1.
 * https://oeis.org/A043932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer k such that the base‑8 representation of
 *            k contains the adjacent digits “01” or “10” but the base‑8
 *            representation of k+1 does not
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Return true if the base‑8 representation of x contains the adjacent
   digits “01” or “10”. */
static bool has_adjacent_01(seq_t x)
{
    seq_t prev = (seq_t)8;          /* sentinel value ≠ 0 or 1 */
    while (x != (seq_t)0) {
        seq_t cur = x % (seq_t)8;
        if ( (prev == (seq_t)0 && cur == (seq_t)1) ||
             (prev == (seq_t)1 && cur == (seq_t)0) )
            return true;
        prev = cur;
        x = x / (seq_t)8;
    }
    return false;
}

/* Memoised sequence generator. */
static seq_t *cache      = NULL;   /* cached values a(i) */
static size_t cache_len  = 0;      /* number of cached terms     */
static size_t cache_cap  = 0;      /* allocated capacity         */
static seq_t next_k      = (seq_t)0;/* next integer to test */

seq_t a(idx_t n)
{
    /* Return cached value if already computed. */
    if ((size_t)n <= cache_len) {
        return cache[n - 1];
    }

    /* Ensure enough storage for the needed terms. */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap > 0 ? cache_cap << 1 : 16;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have n. */
    while (cache_len < (size_t)n) {
        if (has_adjacent_01(next_k) && !has_adjacent_01(next_k + (seq_t)1)) {
            cache[cache_len++] = next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
