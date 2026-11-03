/*
 * A043257  Numbers k such that 6 and 9 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A043257:
 * Numbers k such that 6 and 9 occur juxtaposed in the base‑10 representation of k
 * but not of k−1.
 *
 * The routine generates the sequence by a simple linear search,
 * keeping an internal cache so that repeated calls are fast and
 * calls may arrive in any order.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether a base‑10 number contains the adjacent digits 69 or 96 */
static bool has_adjacent_6_9(seq_t num)
{
    /* Handle the trivial case */
    if (num == 0) return false;

    /* Extract decimal digits into an array.
     * The number of digits for an n‑term number is small (≤10 for n≤10000),
     * so a fixed buffer of 256 digits is more than enough. */
    int digits[256];
    size_t dcnt = 0;
    seq_t tmp = num;

    while (tmp > 0 && dcnt < sizeof(digits)/sizeof(digits[0])) {
        digits[dcnt++] = (int)(tmp % 10);
        tmp = tmp / 10;
    }

    if (dcnt < 2) return false;

    /* Scan from most significant digit to least. */
    for (size_t i = dcnt - 1; i > 0; --i) {
        int high = digits[i];
        int low  = digits[i - 1];
        if (high == 6 && low == 9) return true;
        if (high == 9 && low == 6) return true;
    }
    return false;
}

static seq_t *cache = NULL;        /* Cached values */
static size_t cache_count = 0;     /* Number of cached terms */
static size_t cache_cap = 0;       /* Cache allocation capacity */
static seq_t last_checked = 0;     /* Last number examined during generation */

/* Expand the cache if needed */
static void ensure_cache(size_t needed)
{
    if (cache_count >= needed) return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
    if (!new_cache) abort(); /* allocation failure */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Generate terms until we have at least n terms cached */
static void generate_terms(idx_t n)
{
    ensure_cache((size_t)n);
    while (cache_count < (size_t)n) {
        seq_t candidate = last_checked + 1;
        ++last_checked;

        if (has_adjacent_6_9(candidate) &&
            !has_adjacent_6_9(candidate - 1)) {
            cache[cache_count++] = candidate;
        }
    }
}

/* Public entry point: a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1; behavior below offset not required */
    generate_terms(n);
    return cache[n - 1];
}
