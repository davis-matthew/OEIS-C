/*
 * A118003  a(n) = largest integer <= n which is coprime to A118002(n-1). a(n)
 *          = A118002(n) - A118002(n-1).
 * https://oeis.org/A118003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest integer ≤ n coprime to A118002(n‑1);
 *            A118002(0)=0, A118002(n)=A118002(n‑1)+a(n)
 * Range:     u8 n <= 26, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* malloc/free for C; new/delete for C++ */
#ifdef __cplusplus
#define ALLOC_ARRAY(type, sz) new type[sz]
#define FREE_ARRAY(ptr)       delete[] ptr
#else
#define ALLOC_ARRAY(type, sz) ((type*)malloc((size_t)(sz) * sizeof(type)))
#define FREE_ARRAY(ptr)       free(ptr)
#endif

/* static caches */
static seq_t *a_cache  = NULL;      /* a(1), a(2), …          */
static seq_t *cum_cache = NULL;     /* cum_cache[i] = A118002(i) */
static size_t a_cap   = 0;           /* allocated size for a_cache  */
static size_t cum_cap = 0;           /* allocated size for cum_cache */
static idx_t cache_used = 0;         /* number of values computed   */

/* ensure array has at least new_cap elements; copy when reallocated */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t new_cap)
{
    if (new_cap <= *cap) return;
    seq_t *new_arr = ALLOC_ARRAY(seq_t, new_cap);
    if (*arr) {
        /* copy existing elements */
        for (size_t i = 0; i < *cap; ++i) new_arr[i] = (*arr)[i];
        FREE_ARRAY(*arr);
    }
    *arr = new_arr;
    *cap = new_cap;
}

/* grow caches to be able to answer up to n (n>=1) */
static void grow_cache_to(idx_t n)
{
    if ((idx_t)cache_used >= n) return;

    /* Ensure capacity for a_cache (size n) and cum_cache (size n+1) */
    ensure_capacity(&a_cache, &a_cap, (size_t)n);
    ensure_capacity(&cum_cache, &cum_cap, (size_t)(n + 1));

    /* Initialize base case cum_cache[0] = 0 */
    if (cache_used == 0) cum_cache[0] = 0;

    /* Compute missing values */
    for (idx_t i = cache_used + 1; i <= n; ++i) {
        seq_t Bprev = cum_cache[(size_t)(i - 1)];
        seq_t k     = (seq_t)i;
        /* find largest <= i coprime to Bprev */
        while ((k > (seq_t)0) && (gcd(k, Bprev) != (seq_t)1)) --k;
        a_cache[(size_t)(i - 1)] = k;
        cum_cache[(size_t)i]     = Bprev + k;
    }

    cache_used = n;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, guard */
    grow_cache_to(n);
    return a_cache[(size_t)(n - 1)];
}
