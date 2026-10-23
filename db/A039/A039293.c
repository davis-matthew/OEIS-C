/*
 * A039293  Numbers whose base-6 representation has the same nonzero number of
 *          0's and 1's.
 * https://oeis.org/A039293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th integer whose base‑6 representation has the same *
 *            non‑zero number of 0's and 1's. The sequence starts with a(1) =
 *            6
 * Range:     u8 n <= 36, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;           /* stored qualifying numbers          */
static idx_t cache_len = 0;           /* number of entries cached           */
static idx_t cache_cap = 0;           /* current allocation capacity        */
static seq_t last_checked = 0;        /* last integer examined (for scanning) */

static void extend_cache(idx_t needed)
{
    if (needed <= cache_cap) return;
    idx_t new_cap = (cache_cap > 0 ? cache_cap : 16);
    while (new_cap < needed) new_cap <<= 1;   /* double until enough */
    seq_t *ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!ptr) abort(); /* allocation failed */
    cache = ptr;
    cache_cap = new_cap;
}

/* Returns true iff x’s base‑6 digits contain an equal, positive number
 * of 0's and 1's. */
static bool qualifies(seq_t x)
{
    if (x == 0) return false;    /* 0 never qualifies */
    unsigned zeros = 0, ones = 0;
    seq_t m = x;
    while (m != 0) {
        unsigned d = (unsigned)(m % 6);   /* 0 ≤ d < 6 */
        if (d == 0) ++zeros;
        else if (d == 1) ++ones;
        m = m / 6;
    }
    return (zeros == ones && zeros > 0);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1 */
    if (n <= cache_len) return cache[n - 1];

    /* Generate terms until we have enough. */
    seq_t candidate = last_checked + 1;
    while (cache_len < n) {
        if (qualifies(candidate)) {
            extend_cache(cache_len + 1);
            cache[cache_len++] = candidate;
        }
        ++candidate;
    }
    last_checked = candidate - 1;
    return cache[n - 1];
}
