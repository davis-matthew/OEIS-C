/*
 * A218568  Number of partitions p of n such that max(p)-min(p) = 5.
 * https://oeis.org/A218568
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions p of n such that max(p)-min(p) = 5
 * Range:     u8 n <= 25, u16 n <= 82, u32 n <= 787, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the generating-function description:
 *
 * a(n) = coefficient of x^n in
 *        Sum_{k>=1} x^(2k+5) / Product_{j=0..5} (1 - x^(k+j)).
 * The program precomputes all values up to the largest n requested
 * (therefore it is correct for all n >= 7).  The computation is
 * performed in O(n^2) time with a modest constant; for n <= 10000
 * it runs well under 2 seconds on typical hardware.
 */

#include <stdlib.h>   /* malloc, free, realloc, size_t */
#include <string.h>   /* memset */

#if defined(__cplusplus)
/* In C++ we use operator new/delete to ensure proper construction of
 * objects (e.g. arbitrary–precision integers).  In C we use malloc/free. */
static seq_t *allocate_seq_array(size_t n)
{
    return new seq_t[n];
}
static void free_seq_array(seq_t *p)
{
    delete[] p;
}
#else
static seq_t *allocate_seq_array(size_t n)
{
    return (seq_t *)malloc(n * sizeof(seq_t));
}
static void free_seq_array(seq_t *p)
{
    free(p);
}
#endif

/* Global cache of precomputed sequence values */
static seq_t *a_cache = NULL;     /* array of size cache_max+1 */
static idx_t   cache_max = 0;     /* largest n for which a_cache is valid */

/* Temporary array used during precomputation (reused for each k) */
static seq_t *dp_array = NULL;
static idx_t   dp_size   = 0;

/* Precompute a_cache[0..Nmax] */
static void compute_cache_upto(idx_t Nmax)
{
    /* Allocate cache if necessary */
    if (a_cache == NULL) {
        a_cache = allocate_seq_array(Nmax + 1);
        cache_max = Nmax;
    } else if (Nmax > cache_max) {
        /* Resize cache array when required */
        a_cache = allocate_seq_array(Nmax + 1);
        cache_max = Nmax;
    }

    /* Clear cache entries */
    for (idx_t i = 0; i <= Nmax; ++i)
        a_cache[i] = 0;

    /* Prepare temporary DP array */
    if (dp_size < Nmax + 1) {
        if (dp_array) free_seq_array(dp_array);
        dp_array = allocate_seq_array(Nmax + 1);
        dp_size  = Nmax + 1;
    }

    /* Main double loop over k and the 6 coin values */
    idx_t max_k = (Nmax - 5) / 2;
    for (idx_t k = 1; k <= max_k; ++k) {
        /* Zero DP array */
        for (idx_t i = 0; i <= Nmax; ++i)
            dp_array[i] = 0;
        dp_array[0] = 1;  /* empty partition */

        /* Add the 6 coin types k, k+1, ..., k+5 */
        for (int j = 0; j < 6; ++j) {
            idx_t coin = k + (idx_t)j;
            for (idx_t i = coin; i <= Nmax; ++i)
                dp_array[i] += dp_array[i - coin];
        }

        /* Translate DP results to the sequence a(n) */
        idx_t start_n = 2 * k + 5;
        if (start_n > Nmax) continue;        /* no valid n for this k */
        idx_t limit = Nmax - start_n;
        for (idx_t r = 0; r <= limit; ++r)
            a_cache[start_n + r] += dp_array[r];
    }
}

seq_t a(idx_t n)
{
    /* For indices below the OEIS offset we return 0 (undefined).  The
       function behaves correctly for all n >= 7. */
    if (n < 7) {
        return 0;
    }

    if (a_cache == NULL || n > cache_max) {
        /* Compute (or recompute) cache up to the requested n. */
        compute_cache_upto(n);
    }

    return a_cache[n];
}
