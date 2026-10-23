/*
 * A011800  Number of labeled forests of n nodes each component of which is a
 *          path.
 * https://oeis.org/A011800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence): 2*a(n) = 2*(2*n-1)*a(n-1) - 2*(n-1)^2*a(n-2) + (n-2)*(n-1)*a(n-3)
   a(0) = 1, a(1) = 1, a(2) = 2 */
#include <stdlib.h>

/* seq_t and idx_t are provided by the build; no typedefs are required. */

static seq_t *seq_cache = NULL;     /* caches values 0..max */
static size_t seq_cache_cap = 0;    /* allocated array size */
static idx_t seq_cache_max = 0;     /* highest index currently stored (>=0) */

/* Ensure that seq_cache contains values up to n (inclusive). */
static void
ensure_seq_cache(idx_t n)
{
    if (seq_cache && n <= seq_cache_max) return; /* already have needed values */

    size_t needed = (size_t)n + 1;                /* number of entries required */

    /* Allocate or grow the cache as needed. */
    if (!seq_cache) {
        seq_cache_cap = (needed > 3 ? needed : 3);
        seq_cache = (seq_t *)malloc(seq_cache_cap * sizeof(seq_t));
        /* initialize base cases */
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)1;
        seq_cache[2] = (seq_t)2;
        seq_cache_max = 2;
    } else if (seq_cache_cap < needed) {
        size_t new_cap = seq_cache_cap * 2;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache = tmp;
        seq_cache_cap = new_cap;
    }

    /* Compute new values if needed. */
    if (seq_cache_max < n) {
        for (idx_t i = seq_cache_max + 1; i <= n; ++i) {
            if (i <= 2) {            /* already initialized */
                continue;
            }
            seq_t a_nm1 = seq_cache[i - 1];
            seq_t a_nm2 = seq_cache[i - 2];
            seq_t a_nm3 = seq_cache[i - 3];

            /* Compute each term of the recurrence. */
            seq_t term1 = ((seq_t)2 * (((seq_t)2 * (seq_t)i) - 1)) * a_nm1;
            seq_t term2 = ((seq_t)2 * (seq_t)(i - 1) * (seq_t)(i - 1)) * a_nm2;
            seq_t term3 = ((seq_t)(i - 2) * (seq_t)(i - 1)) * a_nm3;

            seq_t rhs = term1 - term2 + term3;
            seq_cache[i] = rhs / 2;      /* divide by 2 is exact by recurrence */
        }
        seq_cache_max = n;
    }
}

seq_t
a(idx_t n)
{
    ensure_seq_cache(n);
    return seq_cache[n];
}
