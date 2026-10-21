/*
 * A079500  Number of compositions of the integer n in which the first part is
 *          >= the other parts.
 * https://oeis.org/A079500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 36, u64 n <= 69, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A079500
   a(n) = #compositions of n whose first part is ≥ every other part.
   Computation method:
      For each k = 1..n, let f_k(m) = number of compositions of m with parts ≤ k.
      f_k(0) = 1;  f_k(t) = Σ_{i=1}^{min(k,t)} f_k(t-i).
      Then a(n) = Σ_{k=1}^{n} f_k(n-k).
   This routine memoises the sequence for arbitrary values of n. */

#include <stdlib.h>
#if defined(__cplusplus)
#include <cstddef>
#endif

/* Allocate an array of seq_t with default construction (C++) or zero‑initialisation (C). */
static seq_t* alloc_arr(idx_t size) {
#if defined(__cplusplus)
    return new seq_t[(size_t)size];
#else
    return (seq_t*)calloc((size_t)size, sizeof(seq_t));
#endif
}

/* Free an array allocated by alloc_arr. */
static void free_arr(seq_t* arr) {
#if defined(__cplusplus)
    delete[] arr;
#else
    free(arr);
#endif
}

/* Global cache used by a(). */
static seq_t *a_cache = NULL;
static size_t a_cache_size = 0;

/* Build a_cache[0..n] with the sequence values up to n. */
static void compute_upto(idx_t nmax) {
    /* Allocate storage for the cache and a temporary working array. */
    seq_t *cache = alloc_arr((idx_t)nmax + 1u);
    seq_t *dp   = alloc_arr((idx_t)nmax + 1u);

    cache[0] = (seq_t)1;            /* a(0) = 1 */

    for (idx_t k = 1; k <= nmax; ++k) {
        /* dp[0] = 1 for each k; the rest will be recomputed. */
        dp[0] = (seq_t)1;
        seq_t window = 0;

        /* Compute f_k(t) for t = 1 .. nmax. */
        for (idx_t t = 1; t <= nmax; ++t) {
            window += dp[t - 1];
            if (t - 1 >= k)
                window -= dp[t - 1 - k];
            dp[t] = window;
        }

        /* Add f_k(n-k) to the appropriate a(n) values. */
        for (idx_t m = k; m <= nmax; ++m) {
            cache[m] += dp[m - k];
        }
    }

    /* Replace old cache, freeing it if necessary. */
    if (a_cache != NULL)
        free_arr(a_cache);
    a_cache = cache;
    a_cache_size = (size_t)nmax + 1u;

    /* dp is temporary and freed. */
    free_arr(dp);
}

/* Public interface – OEIS A079500. */
seq_t a(idx_t n) {
    if (a_cache == NULL || n >= (idx_t)a_cache_size)
        compute_upto(n);
    return a_cache[n];
}
