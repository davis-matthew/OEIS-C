/*
 * A226471  Numbers k such that k^2 XOR triangular(k) is a triangular number.
 *          XOR is the bitwise logical exclusive-or operator.
 * https://oeis.org/A226471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 45, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   floor_sqrt, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A226471 by enumerating k and checking the
   condition “k^2 XOR triangular(k) is triangular”.                  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;        /* cached k that satisfy the condition */
static size_t cache_len = 0;       /* number of terms stored */
static size_t cache_cap = 0;       /* current capacity of the cache   */
static seq_t next_k = 0;           /* next k to evaluate (seq_t for full generality) */

/* Helper: ensure cache can hold at least 'need' elements.            */
static void grow_cache(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 32;
        if (new_cap < need) new_cap = need;
        seq_t *new_ptr = seq_realloc(cache, cache_cap, new_cap);
        if (!new_ptr) abort();                 /* allocation failed */
        cache = new_ptr;
        cache_cap = new_cap;
    }
}

/* Test whether x is a triangular number.  (8*x + 1) must be a perfect square. */
static bool is_triangular(seq_t x)
{
    seq_t d = ((seq_t)8) * x + 1;
    seq_t s = floor_sqrt(d);
    return s * s == d;
}

/* Main sequence function – 1‑based index. */
seq_t a(idx_t n)
{
    while (cache_len < n) {
        seq_t tri_k = next_k * (next_k + 1) / 2;    /* T(next_k)   */
        seq_t sq     = next_k * next_k;            /* next_k^2    */
        seq_t val    = sq ^ tri_k;                 /* XOR           */
        if (is_triangular(val)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = next_k;
        }
        next_k = next_k + 1;
    }
    return cache[n - 1];
}
