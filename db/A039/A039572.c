/*
 * A039572  Numbers whose base-5 representation has the same number of 2's,
 *          3's and 4's.
 * https://oeis.org/A039572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A039572: numbers whose base‑5 representation has equal numbers of 2's, 3's and 4's.
   Search method: enumerate integers in increasing order and keep those that satisfy
   the digit‑count condition.  Results are cached for fast random accesses. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static seq_t *cache   = NULL;   /* stored sequence values */
static idx_t  clen    = 0;      /* number of terms cached */
static idx_t  cap     = 0;      /* capacity of the cache array */
static uint64_t next   = 0;     /* next integer to test */

/* returns true iff a number has equal counts of digits 2, 3 and 4 in base‑5 */
static bool matches(uint64_t x)
{
    unsigned int c2 = 0, c3 = 0, c4 = 0;

    /* Base‑5 of 0 is just “0”; it has zero 2/3/4 digits → equal. */
    if (x == 0) return true;

    while (x) {
        unsigned int d = (unsigned int)(x % 5);
        x /= 5;
        if (d == 2) ++c2;
        else if (d == 3) ++c3;
        else if (d == 4) ++c4;
    }
    return c2 == c3 && c3 == c4;
}

/* ensures the cache can hold at least ‘need’ terms */
static void ensure_cache_size(idx_t need)
{
    if (cap >= need) return;
    idx_t new_cap = cap ? cap * 2 : 32;
    while (new_cap < need) new_cap *= 2;

    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) abort();   /* minimal error handling */

    if (cache) {
        memcpy(new_arr, cache, clen * sizeof(seq_t));
        free(cache);
    }
    cache = new_arr;
    cap   = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* behaviour below the offset is irrelevant */

    while (clen < n) {
        uint64_t val = next;
        if (matches(val)) {
            ensure_cache_size(clen + 1);
            cache[clen++] = (seq_t)val;
        }
        ++next;
    }
    return cache[n - 1];
}
