/*
 * A223347  3 X 3 X 3 triangular graph without horizontal edges coloring a
 *          rectangular array: number of n X 3 0..5 arrays where 0..5 label
 *          nodes of a graph with edges 0,1 0,2 1,3 1,4 2,4 2,5 and every
 *          array movement to a horizontal or vertical neighbor moves along an
 *          edge of this graph.
 * https://oeis.org/A223347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 91*a(n-2) - 192*a(n-4) + 64*a(n-6) (recurrence valid for
 *            n >= 7) Initial values: a(1)=28, a(2)=236, a(3)=2280,
 *            a(4)=20836, a(5)=202264, a(6)=1851020
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _init[7] = {
    0,              /* a(0) unused */
    28,             /* a(1) */
    236,            /* a(2) */
    2280,           /* a(3) */
    20836,          /* a(4) */
    202264,         /* a(5) */
    1851020         /* a(6) */
};

static seq_t * _cache = NULL;      /* dynamic cache of computed terms */
static idx_t   _cache_len = 0;     /* highest index currently stored */

/* Compute a(n) using the order‑6 linear recurrence with memoisation. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n = 0 */

    /* Grow cache to include index n */
    if (n > _cache_len) {
        size_t old_len = (size_t)_cache_len;
        size_t new_len = (size_t)n;   /* indices 0 .. n   */

        seq_t *tmp = (seq_t *)realloc(_cache, sizeof(seq_t) * (new_len + 1));
        if (!tmp) { exit(1); }        /* allocation failed */
        _cache = tmp;

        /* fill new entries sequentially */
        for (idx_t i = old_len + 1; i <= new_len; ++i) {
            if (i <= 6) {
                _cache[i] = _init[i];
            } else {
                _cache[i] = _cache[i-2] * (seq_t)91
                           - _cache[i-4] * (seq_t)192
                           + _cache[i-6] * (seq_t)64;
            }
        }
        _cache_len = new_len;
    }
    return _cache[n];
}
