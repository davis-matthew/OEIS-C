/*
 * A181999  G.f. A(x) satisfies A(x) = A(x^2) + x*A(x)^2.
 * https://oeis.org/A181999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 65,
 *            big any n
 * Helpers:   free_sequence_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A181999
 *  a(0) = 1, a(1) = 1
 *  a(n) = Σ_{k=0}^{n-1} a(k)*a(n-1-k)                    if n is odd
 *  a(n) = a(n/2) + Σ_{k=0}^{n-1} a(k)*a(n-1-k)            if n is even
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/free_sequence_array.h"

static seq_t *cache = NULL;     /* memoised values */
static idx_t  cache_size = 0;   /* number of allocated elements */
static idx_t  last_computed = 1;/* highest index that has been computed */

/* Allocate an array of seq_t with size `len`.  For C++ we use new[],
 * for C we use calloc.  The array is value‑initialised. */
static seq_t *allocate_seq_array(size_t len) {
#ifdef __cplusplus
    return new seq_t[len]();
#else
    return (seq_t*)calloc(len, sizeof(seq_t));
#endif
}

/* Grow the cache so that indices 0..n are usable.
 * Existing values are preserved.  The two base values a(0) and a(1) are set. */
static void ensure_cache(idx_t n)
{
    if (cache && cache_size > n)
        return;                                   /* already large enough */

    size_t new_size = (size_t)(n + 1);
    seq_t *new_arr = allocate_seq_array(new_size);

    if (cache) {                                  /* copy old values */
        size_t old_size = (size_t)cache_size;
        for (idx_t i = 0; i < old_size; ++i)
            new_arr[i] = cache[i];
        free_sequence_array(cache);
    }

    cache      = new_arr;
    cache_size = new_size;

    /* Initialise the two base terms.  If an index does not exist yet,
       it is left as zero. */
    if (cache_size > 0) cache[0] = 1;
    if (cache_size > 1) cache[1] = 1;
}

/* Sequence function required by the platform */
seq_t a(idx_t n)
{
    ensure_cache(n);                       /* guarantee cache size */

    if (n <= last_computed)               /* already computed */
        return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        idx_t m   = i - 1;                 /* indices run from 0 to m */
        for (idx_t j = 0; j <= m/2; ++j) {
            seq_t prod = cache[j] * cache[m - j];
            if (j == m - j)
                sum += prod;               /* middle term, counted once */
            else {
                sum += prod;               /* off‑diagonal term counted twice */
                sum += prod;
            }
        }
        if ((i & 1) == 0)                 /* even n */
            cache[i] = cache[i/2] + sum;
        else                               /* odd n */
            cache[i] = sum;
    }

    last_computed = n;
    return cache[n];
}
