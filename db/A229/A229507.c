/*
 * A229507  Number of defective 3-colorings of an n X 5 0..2 array connected
 *          diagonally and antidiagonally with exactly one mistake, and colors
 *          introduced in row-major 0..2 order.
 * https://oeis.org/A229507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A229507: Number of defective 3‑colorings of an n×5 array
 * (connected diagonally and anti‑diagonally) with exactly one mistake.
 * Recurrence (valid for n > 9):
 *   a(n) = 30*a(n-1) - 225*a(n-2) - 540*a(n-3) + 8748*a(n-4)
 *          - 9720*a(n-5) - 72900*a(n-6) + 174960*a(n-7) - 104976*a(n-8)
 * Initial values (n = 1..9):
 *   a(1)=0   a(2)=1536   a(3)=31104   a(4)=575424   a(5)=9854784
 *   a(6)=162171072   a(7)=2591476416   a(8)=40561000128   a(9)=624788738496
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* --------------------------------------------------------------------- */
/* Static cache for memoization                                         */
/* --------------------------------------------------------------------- */
static const seq_t init[9] = {
    (seq_t)0ULL,          /* a(1) */
    (seq_t)1536ULL,       /* a(2) */
    (seq_t)31104ULL,      /* a(3) */
    (seq_t)575424ULL,     /* a(4) */
    (seq_t)9854784ULL,    /* a(5) */
    (seq_t)162171072ULL,  /* a(6) */
    (seq_t)2591476416ULL, /* a(7) */
    (seq_t)40561000128ULL,/* a(8) */
    (seq_t)624788738496ULL/* a(9) */
};

static seq_t *cache      = NULL;  /* array of computed terms */
static size_t cache_len  = 0;     /* number of terms currently cached */
static size_t cache_cap  = 0;     /* allocated capacity of cache */

/* --------------------------------------------------------------------- */
static void
ensure_cache(idx_t n)
/* Make sure terms a(1) .. a(n) are cached. */
{
    if ((size_t)n <= cache_len)
        return;

    /* Allocate (or grow) storage */
    if (cache_cap < (size_t)n) {
        seq_t *p = (seq_t*)realloc(cache, n * sizeof(seq_t));
        if (!p && n > 0) abort();       /* unlikely in test environment */
        cache = p;
        cache_cap = (size_t)n;
    }

    /* If cache empty, initialise the first 9 values
     * (or fewer if n < 9, although this branch is used only for n > 9). */
    if (cache_len == 0) {
        size_t init_count = (n < 9) ? (size_t)n : 9;
        for (size_t i = 0; i < init_count; ++i)
            cache[i] = init[i];
        cache_len = init_count;
    }

    /* Compute missing terms using the recurrence */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        /* i corresponds to a(i+1).  Compute a(i+1) from a(i), a(i-1), ... */
        seq_t pos = (seq_t)30 * cache[i-1];
        pos += (seq_t)8748 * cache[i-4];
        pos += (seq_t)174960 * cache[i-7];

        seq_t neg = (seq_t)225 * cache[i-2];
        neg += (seq_t)540 * cache[i-3];
        neg += (seq_t)9720 * cache[i-5];
        neg += (seq_t)72900 * cache[i-6];
        neg += (seq_t)104976 * cache[i-8];

        cache[i] = pos - neg;   /* pos ≥ neg for valid terms */
    }

    cache_len = n;
}

/* --------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0)                       /* offset 1: a(1) is first term */
        return 0;

    if (n <= 9)                       /* return known initial values */
        return init[(size_t)n - 1];

    ensure_cache(n);                  /* compute or fetch from cache */
    return cache[(size_t)n - 1];
}
