/*
 * A170525  Number of reduced words of length n in Coxeter group on 36
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(0) = 1
 *   for 1 ≤ n ≤ 45:   a(n) = 2 + 34 * Σ_{k=0}^{n-1} a(k)
 *   a(46) = 1 + 34 * Σ_{k=1}^{45} a(k) – 595 * a(0)
 *   for n ≥ 47:      a(n) = 34 * Σ_{k=n-45}^{n-1} a(k) – 595 * a(n-46)
 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent cache of computed terms */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;      /* allocated size (≥ last_index + 1) */
    static idx_t   last  = 0;      /* largest index filled so far */
    static seq_t   cumSum = 0;     /* Σ_{k=0}^{last} a(k)   */

    /* Allocate initial storage if necessary */
    if (cache == NULL) {
        cap = 128;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        /* initialise first term */
        cache[0] = (seq_t)1;
        cumSum   = cache[0];
        last     = 0;
    }

    /* Grow cache to cover index n */
    if ((size_t)n >= cap) {
        size_t newcap = (size_t)n + 1;
        newcap += 63;
        newcap &= ~(size_t)63;          /* round up to a multiple of 64 */
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap   = newcap;
    }

    /* Already computed? */
    if (n <= last) {
        return cache[n];
    }

    /* ------------------------------------------------------------------
     *  Compute missing terms for indices (last+1) … n
     * ------------------------------------------------------------------ */
    idx_t i;
    /* 1) 1 ≤ i ≤ 45  (formulas for 0‑to‑45) */
    for (i = last + 1; i <= n && i <= 45; ++i) {
        seq_t val = (seq_t)2 + (seq_t)34 * cumSum;
        cache[i] = val;
        cumSum  += val;
    }
    if (i > n) {                /* finished before reaching 46 */
        last = n;
        return cache[n];
    }

    /* 2) i == 46 */
    /* sum of a[1] … a[45] = cumSum - a0 */
    seq_t sum45_46 = cumSum - cache[0];
    seq_t val46    = (seq_t)34 * sum45_46 - (seq_t)594;   /* 1 – 595 = –594 */
    cache[46] = val46;
    cumSum   += val46;
    ++i;

    if (i > n) {                /* n == 46 */
        last = n;
        return cache[n];
    }

    /* 3) i ≥ 47  (recurrence with a window of 45 terms) */
    /* For i == 47 we need the sum of the 45 terms a[2 … 46] */
    seq_t sum45 = cumSum - cache[0] - cache[1];
    for (; i <= n; ++i) {
        seq_t val = (seq_t)34 * sum45 - (seq_t)595 * cache[i - 46];
        cache[i] = val;
        cumSum  += val;

        /* Update sliding window sum for next iteration */
        sum45  += val;
        sum45  -= cache[i - 45];
    }

    last = n;
    return cache[n];
}
