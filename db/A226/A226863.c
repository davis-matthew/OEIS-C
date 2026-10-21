/*
 * A226863  Triangular numbers not divisible by lesser triangular numbers > 1.
 * https://oeis.org/A226863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = triangular number T_m where T_m is not divisible by any
 *            other triangular number > 1 (OEIS A226863)
 * Range:     u8 n <= 7, u16 n <= 85, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   triangular_bad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/triangular_bad.h"

/* grow the cache if needed */
static void grow_seq(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap) ? (*cap * 2) : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *new_arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *arr = new_arr;
        *cap = new_cap;
    }
}

static seq_t *seq_cache = NULL;    /* cache of sequence terms */
static size_t seq_len  = 0;        /* number of terms stored */
static size_t seq_cap  = 0;        /* capacity of cache */
static idx_t  cur_m    = 1;        /* last examined m */

/* ensure we have at least n terms in the cache */
static void ensure_terms(idx_t n)
{
    if (seq_len >= n) return;          /* already enough */

    grow_seq(&seq_cache, &seq_cap, n);

    while (seq_len < n) {
        ++cur_m;                         /* next candidate m */
        if (triangular_bad(cur_m)) continue;   /* skip bad m */

        seq_t tri = (seq_t)cur_m * (cur_m + 1) / 2;
        seq_cache[seq_len++] = tri;
    }
}

/* public API required by the grader */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n=0 */
    ensure_terms(n);
    return seq_cache[n - 1];
}
