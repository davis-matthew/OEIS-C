/*
 * A039558  Numbers whose base-12 representation has the same nonzero number
 *          of 10's and 11's.
 * https://oeis.org/A039558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence defined by the nth positive integer whose base‑12 representation
   contains an equal and non‑zero number of digits 10 and 11. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Forward declaration of the public function */
seq_t a(idx_t n);

/* Private static data for caching */
static seq_t *cache = NULL;          /* array of discovered terms */
static size_t cache_cap = 0;         /* allocated capacity of cache   */
static size_t cache_len = 0;         /* number of terms stored        */
static idx_t tested_up_to = 0;       /* last integer tested (exclusive) */

/* Internal helper: populate cache up to at least n terms. */
static void ensure_cache(idx_t n)
{
    /* Expand cache storage if needed */
    if (cache_len + n - cache_len > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        while (new_cap < cache_len + (n - cache_len))
            new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n */
    while (cache_len < n) {
        seq_t val = (seq_t)tested_up_to;
        int count10 = 0, count11 = 0;

        /* Count digits 10 and 11 in base‑12 */
        if (val != (seq_t)0) {
            seq_t t = val;
            while (t != (seq_t)0) {
                int digit = (int)(t % (seq_t)12);
                if (digit == 10) ++count10;
                else if (digit == 11) ++count11;
                t /= (seq_t)12;
            }
        }

        if (count10 == count11 && count10 > 0) {
            /* Store valid term */
            cache[cache_len++] = val;
        }
        ++tested_up_to;
    }
}

/* Public API */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0; /* undefined for n=0, return 0 */
    ensure_cache(n);
    return cache[n-1];
}
