/*
 * A213357  E.g.f. satisfies A(x) = 1 + (exp(x) - 1) * A(exp(x) - 1).
 * https://oeis.org/A213357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k * a(k-1) * Stirling2(n,k) (n>0) a(0) = 1
 *            Recurrence uses Stirling numbers of the second kind
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <new>
#endif

/* ------------------------------------------------------------------
 * Helper macros for allocating/freeing arrays that work in both C and
 * C++.  For C++ we use operator new/delete to guarantee that
 * constructors/destructors of arbitrary-precision types are invoked.
 * ------------------------------------------------------------------ */
static inline seq_t *alloc_seq_array(idx_t size) {
#ifdef __cplusplus
    return new seq_t[(size_t)size];
#else
    return (seq_t *)malloc((size_t)size * sizeof(seq_t));
#endif
}

static inline void free_seq_array(seq_t *p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

static inline seq_t *realloc_seq_array(seq_t *old, idx_t old_sz, idx_t new_sz) {
    seq_t *newp = alloc_seq_array(new_sz);
    /* copy elements that exist in both arrays */
    idx_t copy_sz = old_sz < new_sz ? old_sz : new_sz;
    for (idx_t i = 0; i < copy_sz; ++i)
        newp[i] = old[i];
    free_seq_array(old);
    return newp;
}

/* ------------------------------------------------------------------
 * Cached values of the sequence.
 * ------------------------------------------------------------------ */
static seq_t *a_cache = NULL;      /* holds a[0 .. cache_len-1]   */
static idx_t  cache_len = 0;       /* number of terms stored */

/* ------------------------------------------------------------------
 * Cached Stirling row S_prev for n-1:  S_prev[k] = Stirling2(n-1,k)
 * ------------------------------------------------------------------ */
static seq_t *S_prev = NULL;      /* array size S_prev_len */
static idx_t  S_prev_len = 0;

/* ------------------------------------------------------------------
 * Compute a up to index n, filling the caches as needed.
 * ------------------------------------------------------------------ */
static void ensure_up_to(idx_t n) {
    if (n < cache_len)                /* already computed */
        return;

    /* initialisation on first call */
    if (cache_len == 0) {
        a_cache = alloc_seq_array(1);
        a_cache[0] = (seq_t)1;        /* a(0) = 1 */
        cache_len = 1;

        S_prev = alloc_seq_array(1);
        S_prev[0] = (seq_t)1;        /* S(0,0) = 1 */
        S_prev_len = 1;
    }

    /* compute a[cur] for cur = cache_len .. n */
    for (idx_t cur = cache_len; cur <= n; ++cur) {
        /* build Stirling row for n = cur */
        seq_t *S_curr = alloc_seq_array(cur + 1);
        S_curr[0] = (seq_t)0;         /* S(cur,0) = 0 for cur>0 */

        /* recurrence: S(n,k) = S(n-1,k-1) + k * S(n-1,k) */
        for (idx_t k = 1; k <= cur; ++k) {
            seq_t part1 = (k - 1 < S_prev_len) ? S_prev[k - 1] : (seq_t)0;
            seq_t part2 = (k < S_prev_len)     ? S_prev[k]     : (seq_t)0;
            S_curr[k] = part1 + (seq_t)k * part2;
        }

        /* compute a(cur) = Σ_{k=1..cur} k * a(k-1) * S_curr[k] */
        seq_t a_new = (seq_t)0;
        for (idx_t k = 1; k <= cur; ++k)
            a_new += (seq_t)k * a_cache[k - 1] * S_curr[k];

        /* extend a_cache to hold a_new */
        a_cache = realloc_seq_array(a_cache, cache_len, cache_len + 1);
        a_cache[cache_len] = a_new;
        ++cache_len;

        /* free previous Stirling row and keep current one */
        free_seq_array(S_prev);
        S_prev = S_curr;
        S_prev_len = cur + 1;
    }
}

/* ------------------------------------------------------------------
 * Public entry point required by the specification.
 * ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    ensure_up_to(n);
    return a_cache[n];
}
