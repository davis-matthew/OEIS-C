/*
 * A091679  In base 4, numbers n not of the form k + reverse(k) for any k.
 * https://oeis.org/A091679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   rev_base4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates numbers that are NOT of the form k + rev(k)
   in base 4.  It builds a cache of such “good” numbers incrementally: it
   maintains a bitmap of all values up to a dynamically grown bound that
   can be expressed as k + rev(k) in base‑4.  Whenever a value of higher
   index than the current cache is requested, the bound is doubled and
   the bitmap (and the list of good numbers) are rebuilt from scratch.  */

/* Standard headers needed for static memory management. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/rev_base4.h"

/* Forward declaration of the sequence type supplied by the build system. */
/* #include "<path_to_seq_and_idx_definitions>" is omitted because they
   are provided externally. */

/* Static data structures that keep the cache between calls. */
static bool  *bad  = NULL;     /* bad[x] == true iff x = k + rev(k) in base‑4  */
static seq_t *good = NULL;     /* good[i] == i‑th good number                */
static idx_t  bad_len  = 0;     /* size of the bad array (bad_len = bound + 1) */
static size_t good_len = 0;     /* number of good numbers stored               */

/* Rebuild the cache so that it covers all numbers up to new_bound.
   new_bound is the value of the last index that has to be
   represented in the bad bitmap. */
static void recompute_upto(idx_t new_bound)
{
    /* Allocate and zero the bitmap for the new bound. */
    bool *new_bad = (bool *)calloc((size_t)new_bound + 1, sizeof(bool));
    if (!new_bad) abort();  /* BBBG: out of memory */

    /* Mark all numbers that can be written as k + rev(k)
       for 0 ≤ k ≤ new_bound.  If the sum lies within our
       bound, we mark it in the bitmap. */
    for (idx_t k = 0; k <= new_bound; ++k) {
        idx_t sum = k + rev_base4(k);
        if (sum <= new_bound) {
            new_bad[sum] = true;
        }
    }

    /* Allocate space for the list of good numbers.
       In the worst case every number up to the bound is good. */
    seq_t *new_good = (seq_t *)malloc((size_t)new_bound * sizeof(seq_t));
    if (!new_good) {
        free(new_bad);
        abort();
    }

    size_t new_good_len = 0;

    /* Collect all positive integers that are NOT marked bad. */
    for (idx_t x = 1; x <= new_bound; ++x) {
        if (!new_bad[x]) {
            new_good[new_good_len++] = (seq_t)x;
        }
    }

    /* Release the old structures. */
    free(bad);
    free(good);

    /* Install the new structures. */
    bad = new_bad;
    bad_len = new_bound + 1;
    good = new_good;
    good_len = new_good_len;
}

/* Public function: returns the n‑th element of the sequence,
   where the sequence is indexed starting at 0. */
seq_t a(idx_t n)
{
    /* Initialise the cache on the first call. */
    if (!bad) {
        /* A bound of 200 000 comfortably yields >10 000 good numbers
           and is cheap to recompute.  Adjust if higher values are
           required. */
        recompute_upto(200000);
    }

    /* If the requested index lies beyond the current cache, grow the
       bound until it can accommodate n.  The bound is doubled each
       time, guaranteeing O(log n) growth. */
    while (n >= good_len) {
        recompute_upto(bad_len * 2);
    }

    return good[(size_t)n];
}
