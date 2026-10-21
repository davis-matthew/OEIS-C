/*
 * A058511  McKay-Thompson series of class 15D for the Monster group.
 * https://oeis.org/A058511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 29, i16 n <= 95, i32 n <= 325, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A058511
 *  Coefficients of  q^(1/3)*(η(q)/η(q^5))^2
 *  = ∏_{m≥1, 5∤m} (1 - 2 q^m + q^{2m})
 *
 *  The routine computes the coefficient of q^n in this infinite
 *  product by an in‑place convolution.  It memoises the result for all
 *  indices up to the largest previously requested value, so consecutive
 *  calls are fast.
 *  The computation is O(n²) in time and O(n) in memory; it works for all
 *  n ≥ 0 and respects the required type constraints.
 */

#include <stdlib.h>

/* The pointer is NULL until the first use.  cachedUpTo stores the
 * highest index that has been computed (0 means only a[0] is known).
 * If the pointer is NULL we allocate space for a single term. */
static seq_t *cache = NULL;
static idx_t cachedUpTo = 0;

/* Recompute all coefficients up to maxN (inclusive).  The array is
 * reallocated to the correct size; all previous contents are discarded,
 * which is OK because we do a full recomputation. */
static void recompute(idx_t maxN)
{
    /* Allocate or resize the buffer.  Use size_t to avoid overflow. */
    size_t size = (size_t)maxN + 1;
    cache = (seq_t *)realloc(cache, size * sizeof(seq_t));
    if (!cache) abort();            /* out of memory */

    /* Initialise a[0] = 1, a[1..maxN] = 0. */
    cache[0] = (seq_t)1;
    for (idx_t i = 1; i <= maxN; ++i)
        cache[i] = (seq_t)0;

    /* Multiply successively by the factor (1 - 2 q^k + q^{2k}) for all
     * k that are NOT multiples of 5.  The update is performed in-place
     * from high indices to low to preserve the values that are still
     * "old". */
    for (idx_t k = 1; k <= maxN; ++k) {
        if (k % 5 == 0) continue;          /* skip multiples of 5 */

        for (idx_t i = maxN; i > 0; --i) {
            seq_t old    = cache[i];
            seq_t term1  = (i >= k) ? cache[i - k] : (seq_t)0;
            seq_t term2  = (i >= 2 * k) ? cache[i - 2 * k] : (seq_t)0;
            cache[i] = old - ((seq_t)2 * term1) + term2;
        }
        /* cache[0] remains 1 for all k. */
    }

    cachedUpTo = maxN;
}

/* Public function: return the n-th coefficient of the product.  It
 * lazily expands the cached buffer as needed. */
seq_t a(idx_t n)
{
    /* Initialise the cache on the very first call. */
    if (cache == NULL)
        recompute(0);

    if (n > cachedUpTo)
        recompute(n);

    return cache[(size_t)n];
}
