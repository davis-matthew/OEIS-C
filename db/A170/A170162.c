/*
 * A170162  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by the generating function G(t) = (t^39 + 2*t^38 +
 *            2*t^37 + … + 2*t + 1) / (1 - 7*t - 7*t^2 - … - 7*t^38 +
 *            28*t^39). The coefficients satisfy: • a(0) = 1 • for 1 ≤ n ≤ 38
 *            : a(n) = 2 + 7 * Σ_{k=0}^{n-1} a(k) • a(39) = 1 + 7 *
 *            Σ_{k=1}^{38} a(k) – 28 * a(0) • for n ≥ 40 : a(n) = 7*a(n‑...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 none,
 *            big any n
 * Helpers:   alloc_seq_array, compute_recurrence, compute_small_terms,
 *            free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/compute_recurrence.h"
#include "../utils/compute_small_terms.h"

static seq_t *seq_cache = NULL;   /* cache[0..size-1] */
static idx_t  cache_size = 0;     /* number of stored terms */

/* Public sequence function */
seq_t a(idx_t n)
{
    /* If we already have the term cached, return it.  */
    if (cache_size > n) {
        return seq_cache[n];
    }

    /* Allocate a new cache large enough to hold terms up to n.  */
    idx_t new_size = n + 1;
    seq_t *new_cache = alloc_seq_array(new_size);
    /* Copy existing values.  */
    for (idx_t i = 0; i < cache_size; ++i) {
        new_cache[i] = seq_cache[i];
    }

    /* Compute missing small terms up to 39 if needed.  */
    idx_t next = cache_size;
    if (next <= 39) {
        /* Ensure term 0 is present.  */
        if (next == 0) {
            new_cache[0] = (seq_t)1;
            ++next;
        }
        compute_small_terms(new_cache, (next > 39 ? 39 : next));
        /* The above function may compute terms beyond 'next', so adjust.  */
        if (next <= 39 && cache_size < 39) {
            /* after compute_small_terms, cache_size may still be less than 40 */
            next = (next == 40) ? 40 : 40;
        }
    }

    /* Compute recurrence terms for indices ≥ 40.  */
    if (new_size > next) {
        compute_recurrence(new_cache, next, n);
    }

    /* Free old cache and store new one.  */
    free_seq_array(seq_cache);
    seq_cache   = new_cache;
    cache_size  = new_size;

    return seq_cache[n];
}
