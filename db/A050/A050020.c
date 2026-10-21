/*
 * A050020  a(n) = floor(a(n-1)/2) if this is positive and not yet in the
 *          sequence, otherwise a(n) = 10*a(n-1).
 * https://oeis.org/A050020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; for n > 1: a(n) = floor(a(n-1)/2) if that value is
 *            positive and not already in the sequence, otherwise a(n) = 10 *
 *            a(n-1)
 * Range:     u8 n <= 21, u16 n <= 52, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seen
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seen.h"

/* dynamic cache of computed terms (0‑based index: a(1) at cache[0]) */
static seq_t *seq_cache = NULL;
static idx_t  seq_size  = 0;   /* number of terms currently stored (a(1)..a(seq_size)) */
static idx_t  seq_cap   = 0;   /* allocated capacity */

/* grow the cache so that it can hold at least `need` terms */
static void ensure_capacity(idx_t need)
{
    if (need <= seq_cap)
        return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 4;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_ptr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_ptr)          /* allocation failure – abort */
        abort();
    seq_cache = new_ptr;
    seq_cap   = new_cap;
}

/* sequence function, external linkage */
seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for index 0 – return 0 (any value allowed) */
        return (seq_t)0;

    /* initialise cache on first call */
    if (!seq_cache) {
        seq_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!seq_cache)      /* allocation failure – abort */
            abort();
        seq_cache[0] = 1;    /* a(1) = 1 */
        seq_size  = 1;
        seq_cap   = 1;
    }

    /* compute terms until the requested index is available */
    while (seq_size < n) {
        /* ensure sufficient space before writing to cache[seq_size] */
        if (seq_size >= seq_cap)
            ensure_capacity(seq_size + 1);

        seq_t prev = seq_cache[seq_size - 1];
        seq_t cand = prev / 2;          /* floor(prev/2) */

        if (cand > 0 && !seen(seq_cache, seq_size, cand))
            seq_cache[seq_size] = cand;          /* new halved value */
        else
            seq_cache[seq_size] = prev * (seq_t)10;  /* otherwise multiply by 10 */

        ++seq_size;
    }

    return seq_cache[n - 1];   /* a(1) is stored at index 0 */
}
