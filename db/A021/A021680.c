/*
 * A021680  Decimal expansion of 1/676.
 * https://oeis.org/A021680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   Digit a(n) is the n-th digit after the decimal point in the
 *            decimal expansion of 1/676
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is computed by repeated long division: starting with remainder 1, each step multiplies by 10, the quotient is the digit, and the remainder is updated. */

#include <stdlib.h>

static seq_t *cache = NULL;          /* Cached digits */
static size_t cache_len = 0;         /* Number of digits currently cached */
static idx_t  rem = 1;               /* Current remainder (in [0,676)) */
static size_t cache_cap = 0;         /* Allocated capacity of cache */
static const idx_t DEN = 676;        /* Denominator */
static const idx_t BASE = 10;        /* Base for decimal digits */

static void ensure_capacity(size_t needed)
{
    if (needed < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();              /* Out of memory; terminate */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Grow the cache up to index n (inclusive) */
    while (cache_len <= n) {
        idx_t mult = rem * BASE;          /* rem < DEN, so mult < 6760 fits in idx_t */
        seq_t digit = (seq_t)(mult / DEN);/* digit is 0..9 */
        rem = mult % DEN;                 /* Update remainder */

        /* Store the digit, enlarging the cache if necessary */
        ensure_capacity(cache_len + 1);
        cache[cache_len++] = digit;
    }
    return cache[n];
}
