/*
 * A131710  Overlay of Pell numbers: a(n)=A000129(n)+A000129(n-6).
 * https://oeis.org/A131710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A131710: Overlay of Pell numbers
   a(n) = A000129(n) + A000129(n-6) for n ≥ 6,
   a(n) = A000129(n)                for n ≤ 6.
   Pell numbers A000129 are defined by
       P(0) = 0,  P(1) = 1,
       P(k) = 2·P(k-1) + P(k-2)   for k ≥ 2.
   The function caches Pell numbers to support any order of calls
   and works for 8‑ to 128‑bit unsigned types (including arbitrary
   precision).  No external libraries are required beyond standard
   C/C++ headers. */

#include <stdlib.h>
#include <stdbool.h>

/* External integer types supplied by the build */
extern seq_t a(idx_t n);

static seq_t *pell_cache = NULL;            /* Cached Pell numbers */
static idx_t   pell_size  = 0;              /* Current cached size  */
static bool    pell_init  = false;          /* Initialisation flag   */

seq_t a(idx_t n)
{
    /* Initialise cache with the first two Pell numbers */
    if (!pell_init) {
        pell_cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (pell_cache == NULL) {
            /* Allocation failed – abort */
            exit(EXIT_FAILURE);
        }
        pell_cache[0] = (seq_t)0;
        pell_cache[1] = (seq_t)1;
        pell_size  = 2;
        pell_init  = true;
    }

    /* Ensure the cache contains all Pell values up to n */
    if (n >= pell_size) {
        /* Grow the cache to size (n+1) */
        idx_t   new_size = n + 1;
        size_t  new_bytes = (size_t)(new_size) * sizeof(seq_t);
        seq_t * tmp = (seq_t *)realloc(pell_cache, new_bytes);
        if (tmp == NULL) {
            /* Allocation failed – abort */
            exit(EXIT_FAILURE);
        }
        pell_cache = tmp;

        /* Compute new Pell numbers using the recurrence */
        /* Start from the first index not yet computed */
        idx_t start = pell_size;
        if (start < 2) start = 2;
        for (idx_t k = start; k <= n; ++k) {
            pell_cache[k] = (seq_t)2 * pell_cache[k-1] + pell_cache[k-2];
        }
        pell_size = new_size;
    }

    /* Compute the sequence term */
    if (n < 6) {
        return pell_cache[n];
    } else {
        return pell_cache[n] + pell_cache[n-6];
    }
}
