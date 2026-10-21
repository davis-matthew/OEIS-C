/*
 * A221274  Number of n X 4 arrays of occupancy after each element moves to
 *          some horizontal, diagonal or antidiagonal neighbor, with every
 *          occupancy equal to zero or two.
 * https://oeis.org/A221274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A221274  
 *  Formula: a(n) = 7*a(n‑1) + 16*a(n‑2) – 105*a(n‑3) + 108*a(n‑4) + 10*a(n‑5)
 *            – 56*a(n‑6) – 53*a(n‑7) + 23*a(n‑8) – 6*a(n‑9) + a(n‑10)
 *  Initial values (1‑based): a(1)=1, a(2)=25, a(3)=100, a(4)=1089, a(5)=6724,
 *                            a(6)=56644, a(7)=400689, a(8)=3122289, a(9)=23049601,
 *                            a(10)=175350564.
 *
 *  The routine keeps a cache of computed values so that any sequence
 *  of calls (in any order) is handled in O(nmax) time and
 *  O(nmax) space, where nmax is the largest n requested so far.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t *seq_cache = NULL;   /* 1‑based cache of computed terms */
static size_t cache_cap = 0;      /* allocated capacity (>= needed index + 1) */
static idx_t  cache_high = 0;     /* largest index that has been computed */

/* Ensure that indices 1 … n are available in seq_cache. */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;                     /* nothing to do for n = 0 */

    /* The cache must be large enough for indices 1 … n.  The first
     * 10 terms are never requested beyond 10, but we need the array
     * to hold them when we initialise the table.  Therefore the
     * minimum allocation is 11 (for indices 0…10).  *seq_cache[0]
     * is unused. */
    size_t needed_cap = n + 1;
    if (needed_cap < 11) needed_cap = 11;
    if (needed_cap > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(seq_cache, needed_cap * sizeof(seq_t));
        if (!tmp) abort();  /* out of memory */
        seq_cache = tmp;
        cache_cap = needed_cap;
    }

    /* On first use initialise the first ten terms.  After that the
     * cache_high will be at least 10 and the loop below will compute
     * further values only. */
    if (cache_high == 0) {
        seq_cache[1]  = (seq_t)1;
        seq_cache[2]  = (seq_t)25;
        seq_cache[3]  = (seq_t)100;
        seq_cache[4]  = (seq_t)1089;
        seq_cache[5]  = (seq_t)6724;
        seq_cache[6]  = (seq_t)56644;
        seq_cache[7]  = (seq_t)400689;
        seq_cache[8]  = (seq_t)3122289;
        seq_cache[9]  = (seq_t)23049601;
        seq_cache[10] = (seq_t)175350564;
        cache_high = 10;   /* we have indices 1 … 10 ready */
    }

    /* Compute any missing terms up to n.  The recurrence is only
     * defined for n ≥ 11, so the loop starts at max(cache_high+1, 11). */
    for (idx_t i = cache_high + 1; i <= n; ++i) {
        if (i <= 10) continue;   /* safety, although should never happen */

        seq_t term = (seq_t)0;
        term = term + (seq_t)7   * seq_cache[i-1];
        term = term + (seq_t)16  * seq_cache[i-2];
        term = term - (seq_t)105 * seq_cache[i-3];
        term = term + (seq_t)108 * seq_cache[i-4];
        term = term + (seq_t)10  * seq_cache[i-5];
        term = term - (seq_t)56  * seq_cache[i-6];
        term = term - (seq_t)53  * seq_cache[i-7];
        term = term + (seq_t)23  * seq_cache[i-8];
        term = term - (seq_t)6   * seq_cache[i-9];
        term = term + (seq_t)1   * seq_cache[i-10];
        seq_cache[i] = term;
    }

    cache_high = n;
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* out of bounds – not part of the sequence */
    ensure_cache(n);
    return seq_cache[n];
}
