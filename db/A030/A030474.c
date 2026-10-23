/*
 * A030474  n does not have the property that all even digits occur together
 *          and all odd digits occur together.
 * https://oeis.org/A030474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th positive integer that does NOT
   have the property that all even digits occur together and all odd
   digits occur together.
   The implementation searches the positive integers in increasing
   order, keeping a static cache of the first n numbers found.  It
   records only the numbers that violate the property.  The cache is
   extended on demand so that repeated calls in arbitrary order remain
   efficient. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return true if x has the property that all even digits occur
 * in a single contiguous block and all odd digits occur in a single
 * contiguous block.  A number with only one parity automatically
 * satisfies the property. */
static bool property_holds(idx_t x)
{
    /* Extract decimal digits into a temporary array (least
     * significant first). */
    uint8_t digits[64];
    int len = 0;
    if (x == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (x != 0) {
            digits[len++] = (uint8_t)(x % 10);
            x /= 10;
        }
        /* Reverse to most significant first for easier scan. */
        for (int i = 0; i < len / 2; ++i) {
            uint8_t tmp = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = tmp;
        }
    }

    int prevParity = -1;   /* -1 = none, 0 = even, 1 = odd */
    int evenBlocks = 0;
    int oddBlocks  = 0;

    for (int i = 0; i < len; ++i) {
        int p = digits[i] & 1;   /* parity of current digit */
        if (p != prevParity) {   /* parity change */
            if (p == 0)
                ++evenBlocks;
            else
                ++oddBlocks;
            prevParity = p;
        }
    }
    return (evenBlocks <= 1) && (oddBlocks <= 1);
}

/* ----- cache apparatus ----- */

static seq_t *cache = NULL;       /* array of terms */
static size_t cache_size = 0;     /* number of terms stored */
static size_t cache_cap  = 0;     /* allocated capacity */

/* next_candidate holds the integer to test next; it never goes
 * backwards, so repeated calls do not re‑evaluate the same numbers. */
static idx_t next_candidate = 1;

/* Ensure that the cache can hold at least 'need' elements. */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp)
        abort();
    cache = newp;
    cache_cap = new_cap;
}

/* ----- sequence function ----- */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;    /* undefined for n<1, guard anyway */

    /* Generate terms until we have at least n of them. */
    while (cache_size < n) {
        if (!property_holds(next_candidate)) {
            ensure_cache(cache_size + 1);
            cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
