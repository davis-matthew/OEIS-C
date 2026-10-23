/*
 * A043227  Numbers k such that 1 and 4 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS sequence A043227: the n‑th integer k such that the decimal
 * representation of k contains the adjacent digits 1 and 4 in any order,
 * while the representation of k-1 does not contain those adjacent digits. */

/* Standard headers */
#include <stdlib.h>
#include <stdbool.h>

/* Return true iff x contains the substring 14 or 41. */
static bool has_adj14(seq_t x)
{
    seq_t t = x;
    /* Need at least two digits to form a pair. */
    while (t >= 10) {
        seq_t d   = t % 10;   /* current digit (units) */
        t /= 10;             /* remove current digit */
        if ((d == 4 && t % 10 == 1) || (d == 1 && t % 10 == 4))
            return true;
    }
    return false;
}

/* Cache of found terms. */
static seq_t *cache      = NULL; /* array of found terms (increasingly) */
static idx_t cache_size  = 0;    /* number of terms stored */
static idx_t cache_cap   = 0;    /* capacity of the array */

/* Next integer to test. */
static idx_t next_candidate = 1;

/* Ensure the cache can hold at least \"needed\" elements. */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset = 1; caller will ignore n==0 */

    /* Extend the cache until it contains at least n terms. */
    if (cache_size < n) {
        ensure_capacity(n);
        for (idx_t k = next_candidate; ; ++k) {
            seq_t num = (seq_t)k;
            if (has_adj14(num) && !has_adj14(num - (seq_t)1)) {
                cache[cache_size++] = num;
                if (cache_size == n) {
                    next_candidate = k + 1;
                    break;
                }
            }
        }
    }
    return cache[n - 1];
}
