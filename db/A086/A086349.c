/*
 * A086349  On a 3 X 3 board, the number of n-move paths for a chess king.
 * https://oeis.org/A086349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=9, a(2)=40, a(3)=200 a(n) =
 *            2*a(n-1)+12*a(n-2)+8*a(n-3) for n>3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* cached values, allocated as size_t numbers */
static size_t  cap   = 0;      /* capacity of the cache (number of slots)   */
static idx_t   last  = (idx_t)-1; /* index of the last computed value (inclusive) */

/* Allocate space for at least 'need' elements (indices 0..need-1). */
static void grow_capacity(size_t need)
{
    if (cap >= need)
        return;
    size_t new_cap = cap ? cap : 4;
    if (new_cap < need) {
        /* grow by the smallest power of two ≥ need */
        while (new_cap < need)
            new_cap <<= 1;
    }
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cap = new_cap;
}

/* Compute values up to index n (inclusive).  Uses memoization. */
static void ensure_up_to(idx_t n)
{
    if (last == (idx_t)-1) {            /* first call: initialise 0..3 */
        grow_capacity(4);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        cache[2] = (seq_t)40;
        cache[3] = (seq_t)200;
        last = 3;
    }

    if ((idx_t)n <= last)               /* already computed */
        return;

    grow_capacity((size_t)n + 1);       /* ensure enough storage */

    /* Compute missing terms one by one. */
    for (size_t i = (size_t)last + 1; i <= (size_t)n; ++i) {
        if (i <= 3) {                    /* safety; initial values already set */
            continue;
        }
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        cache[i] = ((seq_t)2 * a1) + ((seq_t)12 * a2) + ((seq_t)8 * a3);
    }
    last = (idx_t)n;
}

seq_t a(idx_t n)
{
    ensure_up_to(n);
    return cache[(size_t)n];
}
