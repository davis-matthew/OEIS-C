/*
 * A170221  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A170221
 *  Sequence defined by the generating function
 *
 *          G(t) = (t^40 + 2*t^39 + 2*t^38 + ... + 2*t + 1) /
 *                 (171*t^40 - 18*t^39 - 18*t^38 - ... - 18*t + 1)
 *
 *  This yields a linear recurrence of order 40:
 *
 *          a(n) = 18 * Σ_{i=1}^{39} a(n-i) - 171 * a(n-40)     (n ≥ 40)
 *
 *  The first 40 terms are obtained from the numerator via the relation
 *          a(n) - 18 * Σ_{i=1}^{min(39,n)} a(n-i) = N_n,
 *      where N_0 = 1, N_n = 2 for 1 ≤ n ≤ 39, and N_40 = 1.
 *
 *  The implementation below uses a static cache and a circular buffer
 *  to compute terms in O(n) time with a small constant factor.
 *
 *  External linkage: a(idx_t n) -> seq_t
 */
#include <stdlib.h>

extern seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of terms stored (cache_size == next index to compute) */
    static idx_t cache_cap  = 0;   /* allocated capacity */

    /* Ensure cache can hold up to n */
    idx_t needed = n + (idx_t)1;
    if (cache_cap < needed) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 256;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure; caller responsible */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If we already have the requested term, return it */
    if (cache_size > n) return cache[n];

    /* Circular buffer holding the last up to 39 terms */
    static seq_t last[39] = {0};
    static idx_t head = 0;     /* next position to write */
    static idx_t len_last = 0; /* number of elements currently in buffer (≤39) */
    static seq_t sum_last = 0; /* sum of elements in the buffer */

    /* Compute missing terms from cache_size up to n */
    for (idx_t k = cache_size; k <= n; ++k) {
        seq_t val;

        if (k == 0) {
            /* a(0) = 1 */
            val = (seq_t)1;
        } else if (k <= 39) {
            /* For 1 ≤ k ≤ 39: a(k) = 2 + 18 * sum_last */
            val = (seq_t)2 + (seq_t)18 * sum_last;
        } else if (k == 40) {
            /* a(40) = 1 + 18 * sum_last - 171 * a(0) */
            val = (seq_t)1 + (seq_t)18 * sum_last - ((seq_t)171 * cache[0]);
        } else {
            /* For k > 40: a(k) = 18 * sum_last - 171 * a(k-40) */
            val = (seq_t)18 * sum_last - ((seq_t)171 * cache[k - 40]);
        }

        /* Store the new term */
        cache[k] = val;

        /* Update the circular buffer and running sum */
        if (len_last == 39) {
            /* Remove the oldest term from sum */
            sum_last -= last[head];
        } else {
            /* Buffer still growing */
            ++len_last;
        }

        /* Insert the new term */
        last[head] = val;
        sum_last += val;
        head = (head + 1) % 39;  /* advance head */
    }

    /* Update the cache size to reflect new entries */
    cache_size = n + 1;

    /* Return the requested term */
    return cache[n];
}
