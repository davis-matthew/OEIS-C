/*
 * A094547  a(n) = A019309(n)/4 for n >= 1.
 * https://oeis.org/A094547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(2*n) = 4*a(2*n-1) - a(n); a(2*n+1) = 4*a(2*n)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A094547, index starting at 1.  The function is memoised
   so that calls in any order are efficient. */

#include <stdlib.h>
#include <stddef.h>

/* external types supplied by the build system */
extern seq_t a(idx_t n);

static seq_t *a_cache = NULL;
static idx_t  a_cache_len = 0;  /* number of slots in a_cache (max index + 1) */

/* Helper: compute a value for a particular index */
static seq_t compute_a(idx_t i)
{
    seq_t val = a_cache[i-1] * (seq_t)4;   /* 4*a(i-1) */
    if ((i & 1u) == 0)                    /* even index: subtract a(i/2) */
        val = val - a_cache[i/2];
    return val;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* out of range; not expected */

    /* Ensure the cache covers the requested index */
    if (a_cache_len <= n) {
        /* new size needed */
        size_t new_len = (size_t)n + 1;   /* indices 0 .. n used; index 0 unused */
        seq_t *tmp = a_cache;
        if (a_cache == NULL) {
            tmp = (seq_t *)malloc(new_len * sizeof(seq_t));
            if (!tmp) abort();            /* failure to allocate */
        } else {
            tmp = (seq_t *)realloc(tmp, new_len * sizeof(seq_t));
            if (!tmp) abort();
        }
        a_cache = tmp;

        /* initialise new entries */
        if (a_cache_len == 0) {           /* first allocation */
            a_cache[1] = (seq_t)1;        /* a(1) */
            if (n >= 2) {
                for (size_t i = 2; i <= (size_t)n; ++i) {
                    a_cache[i] = compute_a((idx_t)i);
                }
            }
        } else {                          /* grow the cache */
            for (size_t i = a_cache_len; i <= (size_t)n; ++i) {
                a_cache[i] = compute_a((idx_t)i);
            }
        }
        a_cache_len = new_len;
    }

    return a_cache[n];
}
