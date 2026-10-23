/*
 * A152029  a(n) = 2^n*(2*n)!/((n+1)!).
 * https://oeis.org/A152029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * (2*n)! / ((n+1)!) Recurrence: a(0) = 1; a(n) =
 *            a(n-1) * 4*n*(2*(n-1)+1) / (n+1) (n ≥ 1) This implementation
 *            memoises the sequence up to the largest value needed. It uses
 *            the standard library realloc/alloc for storage
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *CACHE = NULL;   /* Store previously computed terms */
static size_t CACHE_CAP = 0;  /* Current allocated size in elements   */
static idx_t  CACHE_SIZE = 0; /* Number of values currently stored    */

/* Ensure CACHE can hold at least indices 0 … n. */
static void ensure_cache(idx_t n)
{
    size_t required = (size_t)(n + 1);
    if (required <= CACHE_CAP) return;

    size_t new_cap = required;
    if (new_cap < 16) new_cap = 16;

    if (CACHE == NULL) {
        CACHE = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        CACHE = (seq_t *)realloc(CACHE, new_cap * sizeof(seq_t));
    }
    CACHE_CAP = new_cap;
}

/* Compute the sequence up to and including n, filling the cache. */
static void compute_up_to(idx_t n)
{
    /* Initialise a(0) if the cache is empty. */
    if (CACHE_SIZE == 0) {
        ensure_cache(0);
        CACHE[0] = (seq_t)1;   /* a(0) = 1 */
        CACHE_SIZE = 1;
    }

    /* Compute new terms. */
    for (idx_t k = CACHE_SIZE; k <= n; ++k) {
        seq_t prev = CACHE[k - 1];
        seq_t next = prev;
        next *= (seq_t)4;
        next *= (seq_t)k;
        next *= (seq_t)(2 * (k - 1) + 1);
        next /= (seq_t)(k + 1);
        CACHE[k] = next;
    }

    CACHE_SIZE = n + 1;
}

/* Public interface required by the problem statement. */
seq_t a(idx_t n)
{
    if (n < CACHE_SIZE) return CACHE[n];
    ensure_cache(n);
    compute_up_to(n);
    return CACHE[n];
}
