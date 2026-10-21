/*
 * A080703  a(1)=5; for n>1, a(n) = smallest number > a(n-1) such that the
 *          condition "if n is in the sequence then a(n) is a multiple of 4"
 *          is satisfied.
 * https://oeis.org/A080703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* a(1)=5; for n>1, a(n) = smallest number > a(n-1) such that if n is in the
   sequence then a(n) is a multiple of 4. */

static seq_t *seq_cache = NULL;   /* cache for a(1…k) */
static size_t seq_cap  = 0;       /* capacity of seq_cache */
static idx_t seq_len    = 0;      /* number of terms computed (last index) */

/*  Ensure seq_cache has room for indices up to req (1‑based). */
static void ensure_seq_cap(idx_t req)
{
    if (seq_cache == NULL) {
        seq_cap = 16;
        seq_cache = (seq_t*)calloc(seq_cap, sizeof(seq_t));
    }
    if (seq_cap <= (size_t)req) {
        size_t new_cap = seq_cap * 2;
        if (new_cap <= (size_t)req) new_cap = (size_t)req + 1;
        seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_cache = tmp;
        seq_cap = new_cap;
    }
}

/*  Check whether the value val occurs in the already computed part of the
    sequence (values are strictly increasing, so a binary search works). */
static bool contains_value(idx_t val)
{
    size_t lo = 1;
    size_t hi = (size_t)seq_len;   /* highest valid index */
    seq_t target = (seq_t)val;
    while (lo <= hi) {
        size_t mid = (lo + hi) >> 1;
        seq_t v = seq_cache[mid];
        if (v == target) return true;
        if (v < target)  lo = mid + 1;
        else             hi = mid - 1;
    }
    return false;
}

/*  Sequence function.  It is memoised and works for calls in arbitrary order. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1 */

    while (seq_len < n) {
        idx_t idx = seq_len + 1;    /* compute next index */
        ensure_seq_cap(idx);

        seq_t cand;
        if (seq_len == 0) {         /* a(1) */
            cand = (seq_t)5;
        } else {
            seq_t prev = seq_cache[(size_t)seq_len];
            cand = prev + (seq_t)1; /* must be > previous value */

            /* If the index itself appears as a value of the sequence,
               enforce the "multiple of 4" condition. */
            if (contains_value(idx)) {
                seq_t rem = cand % (seq_t)4;
                if (rem != (seq_t)0)
                    cand += (seq_t)(4 - rem);
            }
        }

        seq_cache[(size_t)idx] = cand;
        seq_len = idx;
    }

    return seq_cache[(size_t)n];
}
