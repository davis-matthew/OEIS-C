/*
 * A026122  a(n) is the number of (s(0),s(1),...,s(n)) such that every s(i) is
 *          a nonnegative integer, s(0) = 1, s(n) = 1, |s(1) - s(0)| = 1,
 *          |s(i) - s(i-1)| <= 1 for i >= 2. Also a(n) = T(n,n), where T is
 *          the array in A026120.
 * https://oeis.org/A026122
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A026122: a(n) = # of walks of length n on non‑negative integers
   starting at 1, ending at 1, with first step ±1, subsequent steps ∈{−1,0,+1}.
   Computed by dynamic programming in O(n²) time and O(n) space. */

#include <stdlib.h>
#include <string.h>

static seq_t *prev = NULL;          /* dp for step i-1 */
static seq_t *cur  = NULL;          /* dp for step i   */
static idx_t  max_alloc = 0;        /* capacity of allocated arrays */

/* Return a(n) for n ≥ 2.  For n < 2 the value is undefined. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;            /* below OEIS offset */

    /* Allocate or enlarge DP arrays if necessary. */
    size_t needed = (size_t)n + 3;  /* heights 0 … n+1 inclusive */
    if (!prev || needed > max_alloc) {
        free(prev);
        free(cur);
        prev = (seq_t *)calloc(needed, sizeof(seq_t));
        cur  = (seq_t *)calloc(needed, sizeof(seq_t));
        if (!prev || !cur) abort();
        max_alloc = needed;
    }

    /* Initialise: step 0, only height 1 is reachable. */
    memset(prev, 0, max_alloc * sizeof(seq_t));
    prev[1] = (seq_t)1;

    /* Step 1: first step must be ±1 (cannot stay).  Reach heights 0 and 2. */
    memset(cur, 0, max_alloc * sizeof(seq_t));
    cur[0] = (seq_t)1;
    cur[2] = (seq_t)1;
    /* swap prev and cur: now prev holds dp for step 1 */
    seq_t *tmp = prev; prev = cur; cur = tmp;

    /* For steps i = 2 … n: allowed steps −1,0,+1. */
    for (idx_t i = 2; i <= n; ++i) {
        /* Clear current array. */
        memset(cur, 0, max_alloc * sizeof(seq_t));

        /* Heights reachable after i steps are 0 … i+1. */
        size_t hmax = (size_t)i + 1;
        for (size_t h = 0; h <= hmax; ++h) {
            seq_t stay  = prev[h];
            seq_t up    = (h > 0) ? prev[h - 1] : (seq_t)0;
            seq_t down  = prev[h + 1];
            cur[h] = stay + up + down;
        }
        /* Next iteration: cur becomes prev. */
        tmp = prev; prev = cur; cur = tmp;
    }

    /* After completing n steps, answer is number of ways to end at height 1. */
    return prev[1];
}
