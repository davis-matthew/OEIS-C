/*
 * A200660  Sum of the number of arcs describing the set partitions of
 *          {1,2,...,n}.
 * https://oeis.org/A200660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} Stirling₂(n,k) * k * (n-k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 23, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a running table of Stirling 2‑numbers */
/* and evaluates the sum incrementally up to the required n.     */

#include <stdlib.h>
#include <stddef.h>   /* size_t */

static seq_t *stirling = NULL;  /* array S(k) for current n   */
static size_t stirling_cap = 0;

static seq_t *cache = NULL;     /* cached a(n) values          */
static size_t cache_cap = 0;

static idx_t current_max = 0;   /* largest n that has been computed */

/*--- memory helpers -----------------------------------------------------*/

static void grow_stirling(size_t new_cap) {
    if (new_cap <= stirling_cap)
        return;

    if (!stirling) {
#ifdef __cplusplus
        stirling = new seq_t[new_cap];
#else
        stirling = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
        if (!stirling)
            abort();      /* OOM */
        for (size_t i = 0; i < new_cap; ++i)
            stirling[i] = 0;
        stirling_cap = new_cap;
        return;
    }

#ifdef __cplusplus
    seq_t *new_arr = new seq_t[new_cap];
    size_t old_cap = stirling_cap;
    for (size_t i = 0; i < old_cap; ++i)
        new_arr[i] = stirling[i];
    for (size_t i = old_cap; i < new_cap; ++i)
        new_arr[i] = 0;
    delete[] stirling;
    stirling = new_arr;
#else
    size_t old_cap = stirling_cap;
    seq_t *tmp = (seq_t*)realloc(stirling, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();      /* OOM */
    stirling = tmp;
    for (size_t i = old_cap; i < new_cap; ++i)
        stirling[i] = 0;
#endif

    stirling_cap = new_cap;
}

static void grow_cache(size_t new_cap) {
    if (new_cap <= cache_cap)
        return;

    if (!cache) {
#ifdef __cplusplus
        cache = new seq_t[new_cap];
#else
        cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
        if (!cache)
            abort();      /* OOM */
        /* cache[0] is unused but set for symmetry */
        cache[0] = 0;
        cache_cap = new_cap;
        return;
    }

#ifdef __cplusplus
    seq_t *new_arr = new seq_t[new_cap];
    size_t old_cap = cache_cap;
    for (size_t i = 0; i < old_cap; ++i)
        new_arr[i] = cache[i];
    delete[] cache;
    cache = new_arr;
#else
    size_t old_cap = cache_cap;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();      /* OOM */
    cache = tmp;
#endif

    cache_cap = new_cap;
}

/*--- core computation ----------------------------------------------------*/

static void ensure_up_to(idx_t target) {
    if (target <= current_max)
        return;

    /* first time initialisation */
    if (!stirling) {
        size_t init_cap = (size_t)target + 1;
        grow_stirling(init_cap);
        grow_cache(init_cap);
        stirling[0] = 1;      /* S(0,0) = 1 */
        cache[0] = 0;         /* unused slot */
        current_max = 0;
    }

    if (stirling_cap <= target)
        grow_stirling((size_t)target + 1);
    if (cache_cap <= target)
        grow_cache((size_t)target + 1);

    /* incremental computation */
    for (idx_t n = current_max + 1; n <= target; ++n) {
        seq_t T1 = 0;   /* Σ k * S(n,k) */
        seq_t U  = 0;   /* Σ k² * S(n,k) */

        /* update Stirling numbers in place, descending k */
        for (idx_t k = n; k > 0; --k) {
            seq_t prev     = stirling[k];          /* S(n-1,k) */
            seq_t newVal   = (seq_t)k * prev + stirling[k-1]; /* S(n,k) */
            stirling[k] = newVal;

            seq_t kSeq = (seq_t)k;
            T1 += kSeq * newVal;
            U  += kSeq * kSeq * newVal;
        }
        stirling[0] = 0;        /* S(n,0) = 0 for n>0 */

        cache[n] = (seq_t)n * T1 - U;   /* a(n) = n*T1 – U */
    }
    current_max = target;
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;                /* sequence is defined for n ≥ 1 */
    ensure_up_to(n);
    return cache[n];
}
