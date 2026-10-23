/*
 * A021131  Decimal expansion of 1/127.
 * https://oeis.org/A021131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the n‑th term of the decimal expansion of 1/127, where
 *            a(0) = 0 (the integer part), a(1) = 0 (first decimal digit),
 *            a(2) = 7, a(3) = 8, …
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a long division of 1 by 127, caching digits for reuse. */
#include <stdlib.h>
#include <stdint.h>

#define DIVISOR 127ULL

/* Cache for computed digits.  digit_cache[i] holds the i‑th
   decimal digit of 1/127 (1 ≤ i).  digit_cache[0] is unused. */
static seq_t *digit_cache = NULL;   /* array of digits, 1‑based */
static size_t cache_cap = 0;        /* number of elements allocated */
static idx_t cached_up_to = 0;      /* highest index computed so far (≥0) */
static uint64_t rem = 1;            /* current remainder, starting with 1 */

/* Ensure that digits up to index need (inclusive) are computed. */
static void
ensure_cache(idx_t need)
{
    /* Initial allocation if necessary.  We allocate at least two slots
       so that digit_cache[1] is available. */
    if (digit_cache == NULL) {
        cache_cap = 2;
        digit_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!digit_cache) abort();
        digit_cache[0] = 0;           /* unused, keep zero for safety */
        digit_cache[1] = 0;           /* first decimal digit (will be overwritten) */
        cached_up_to = 0;             /* nothing yet computed */
    }

    if (need <= cached_up_to)  /* nothing new to compute */
        return;

    /* Grow the cache if needed. */
    if ((size_t)need + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 2;
        while ((size_t)need + 1 > new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(digit_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        digit_cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute digits from cached_up_to+1 up to need (inclusive). */
    for (idx_t i = cached_up_to + 1; i <= need; ++i) {
        uint64_t t = rem * 10ULL;
        digit_cache[i] = (seq_t)(t / DIVISOR);   /* the i‑th digit */
        rem = t % DIVISOR;                       /* remainder for next step */
    }
    cached_up_to = need;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* integer part of 1/127 */
        return 0;

    /* a(1) corresponds to the first decimal digit, which is stored
       at digit_cache[1].  In general a(n) = digit_cache[n+1]. */
    ensure_cache(n + 1);
    return digit_cache[n + 1];
}
