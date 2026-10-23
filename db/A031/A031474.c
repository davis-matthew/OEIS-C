/*
 * A031474  Numbers whose base-5 representation has one fewer 0 than 4's.
 * https://oeis.org/A031474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the n-th positive integer whose base‑5 representation
 * contains exactly one more digit 4 than digit 0.
 *
 * Implementation detail: brute‑force search.  The sequence grows with
 * density ≈ 1/√log_5(x), so for n = 10 000 the values stay below a few
 * × 10^4; a linear scan is far below the 2‑second limit.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  test whether a number satisfies the digit condition                */
/* ------------------------------------------------------------------ */
static bool cond(seq_t x)
{
    seq_t y = x;
    unsigned zeros = 0, fours = 0;

    while (y != 0) {
        seq_t d = y % 5;
        if (d == 0)      zeros++;
        else if (d == 4) fours++;
        y = y / 5;
    }

    /* need at least one 4 and exactly one more 4 than 0 */
    return (fours > 0) && (zeros + 1 == fours);
}

/* ------------------------------------------------------------------ */
/*  dynamic cache for previously computed terms                        */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;   /* array of computed values */
static size_t cache_size = 0; /* number of values stored */
static size_t cache_cap   = 0;/* allocated size */
static seq_t last_checked = 0;/* last integer examined    */

/* ensure the cache can hold at least `needed` elements */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* out of memory */
    cache    = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  public function                                                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for index 0 */
    size_t req = (size_t)n;        /* cast for comparison */

    /* extend cache until it contains at least n terms */
    while (cache_size < req) {
        seq_t candidate = last_checked + 1; /* next integer to test */

        if (cond(candidate)) {
            ensure_capacity(cache_size + 1);
            cache[cache_size++] = candidate;
        }

        last_checked = candidate; /* remember how far we went */
    }

    /* indices are 1‑based, array is 0‑based */
    return cache[n - 1];
}
