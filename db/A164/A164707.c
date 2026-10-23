/*
 * A164707  A positive integer n is included if all runs of 1's in binary n
 *          are of the same length.
 * https://oeis.org/A164707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 3289, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   good_run_length_all_equal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A164707: numbers whose binary 1‑runs all have the same length.
 *  This routine generates the sequence by scanning successive integers
 *  and keeping those that satisfy the property.  The values are cached
 *  so each term is computed at most once, regardless of call order.   */

#include <stdlib.h>
#include <assert.h>
#include "../utils/good_run_length_all_equal.h"

/* ----- global cache ----------------------------------------------------- */
static seq_t *cache = NULL;       /* cache of computed terms          */
static size_t cache_len = 0;      /* number of terms stored          */
static size_t cache_cap = 0;      /* current cache capacity          */
static idx_t next_candidate = 1;  /* next integer to test            */

/* ----- helper: enlarge cache if needed -------------------------------- */
static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();            /* memory allocation failed */

    cache = tmp;
    cache_cap = new_cap;
}

/* ----- public function -------------------------------------------------- */
seq_t a(idx_t n)
{
    assert(n >= 1);               /* index must be positive          */

    /* generate terms until we have enough for the requested index */
    while (cache_len < (size_t)n) {
        /* find next number that satisfies the run‑length property   */
        while (!good_run_length_all_equal(next_candidate))
            ++next_candidate;

        /* store the term */
        ensure_capacity(cache_len + 1);
        cache[cache_len++] = (seq_t)next_candidate;

        /* move on to the next integer for the next search */
        ++next_candidate;
    }

    return cache[(size_t)n - 1];
}
