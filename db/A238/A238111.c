/*
 * A238111  Twice the large Schroeder numbers A006318.
 * https://oeis.org/A238111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * S(n), where S(n) are the large Schröder numbers with
 *            S(0) = 1 and recurrence S(n) = S(n-1) + Σ_{k=0}^{n-1}
 *            S(k)*S(n-1-k) for every n ≥ 1. This definition yields a(0)=2,
 *            a(1)=4, a(2)=12, … as required
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached sequence of large Schröder numbers S(n). */
static seq_t *schroeder_cache = NULL;   /* array of S values */
static idx_t  cache_len  = 0;           /* number of elements stored (highest index + 1) */
static idx_t  cache_cap  = 0;           /* allocated capacity */

/* Ensure that schroeder_cache can hold elements up to index 'n'. */
static void
ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;
    /* Grow capacity: double until sufficient. */
    idx_t new_cap = (n + 1) * 2;
    seq_t *new_ptr = (seq_t *)realloc(schroeder_cache,
                                      new_cap * sizeof(seq_t));
    if (!new_ptr) {
        /* Out of memory; on failure we abort the program. */
        abort();
    }
    schroeder_cache = new_ptr;
    cache_cap = new_cap;
}

/* Compute S(n) for all values up to n using the recurrence, and
   store them in schroeder_cache. */
static void
compute_up_to(idx_t n)
{
    if (cache_len == 0) {
        /* initialise S(0)=1 */
        schroeder_cache[0] = (seq_t)1;
        cache_len = 1;
    }
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = schroeder_cache[i - 1];
        for (idx_t k = 0; k < i; ++k) {
            sum += schroeder_cache[k] * schroeder_cache[i - 1 - k];
        }
        schroeder_cache[i] = sum;
    }
    cache_len = n + 1;
}

/* Entry point for the sequence. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    compute_up_to(n);
    seq_t result = schroeder_cache[n];
    result *= (seq_t)2;  /* final multiplication by 2 */
    return result;
}
