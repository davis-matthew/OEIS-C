/*
 * A061482  a(1) = 1, a(2) = 2, a(n) = sum of products of previous terms
 *          taking n-2 at a time.
 * https://oeis.org/A061482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 2, * a(n) = sum of products of previous terms
 *            taken n-2 at a time
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#ifdef __cplusplus
#include <algorithm>
#endif

/* Allocation helpers compatible with both C and C++ builds. */
static seq_t *seq_alloc(size_t n) {
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}
static void seq_free(seq_t *p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* The sequence function with memoisation. */
seq_t a(idx_t n)
{
    /* Persistant state across calls. */
    static seq_t *a_cache   = NULL; /* 1‑based index of computed terms */
    static size_t a_cap     = 0;     /* allocated capacity of a_cache   */
    static size_t a_len     = 0;     /* number of terms currently stored */
    static seq_t *e_cache   = NULL; /* elementary symmetric sums; e[0]=1 */
    static size_t e_cap     = 0;     /* allocated capacity of e_cache  */
    static size_t e_len     = 0;     /* number of e indices currently stored */

    /* Helper: ensure a_cache holds at least 'need' indices (1‑based). */
    if (a_cap <= (size_t)n + 3) {
        size_t new_cap = (a_cap ? a_cap * 2 : 8) + 8;
        if (new_cap < (size_t)n + 4) new_cap = (size_t)n + 4;
        seq_t *newp = seq_alloc(new_cap);
        if (a_cache) {
            for (size_t i = 1; i <= a_len; ++i) newp[i] = a_cache[i];
            seq_free(a_cache);
        }
        a_cache = newp;
        a_cap   = new_cap;
    }

    /* Helper: ensure e_cache holds at least 'need+1' indices (0‑based). */
    if (e_cap <= (size_t)n + 4) {
        size_t old_cap = e_cap;
        size_t new_cap = (e_cap ? e_cap * 2 : 8) + 8;
        if (new_cap < (size_t)n + 5) new_cap = (size_t)n + 5;
        seq_t *newp = seq_alloc(new_cap);
        if (e_cache) {
            for (size_t i = 0; i < old_cap; ++i) newp[i] = e_cache[i];
            seq_free(e_cache);
        }
        /* initialise new slots to zero */
        for (size_t i = old_cap; i < new_cap; ++i) newp[i] = (seq_t)0;
        e_cache = newp;
        e_cap   = new_cap;
        e_len   = new_cap;
    }

    /* Fast paths for the tiny initial values. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* Initialise the first two terms lazily. */
    if (a_len == 0) {
        /* ensure_e_cache has at least indices 0..2 */
        if (e_cache == NULL) {
            e_cache = seq_alloc(8);
            e_cap   = 8;
            e_len   = 8;
            for (size_t i = 0; i < 8; ++i) e_cache[i] = (seq_t)0;
        }
        e_cache[0] = (seq_t)1;               /* e_0 = 1 */
        /* a(1) */
        a_cache[1] = (seq_t)1;
        a_len = 1;
        /* update e: e_1 += a(1)*e_0 */
        e_cache[1] += (seq_t)1 * e_cache[0];
        /* a(2) */
        a_cache[2] = (seq_t)2;
        a_len = 2;
        /* update e for k=2,1 */
        e_cache[2] += (seq_t)2 * e_cache[1];
        e_cache[1] += (seq_t)2 * e_cache[0];
    }

    /* Compute terms up to the requested index. */
    while (a_len < (size_t)n) {
        /* new term is the (a_len-1)-th elementary sum of existing terms */
        seq_t next_val = e_cache[a_len - 1];
        ++a_len;
        a_cache[a_len] = next_val;
        /* ensure e_cache has space for index a_len (new highest) */
        if (e_len <= a_len) {
            size_t old_cap = e_cap;
            size_t new_cap = e_cap * 2 + 8;
            if (new_cap < a_len + 2) new_cap = a_len + 2;
            seq_t *newp = seq_alloc(new_cap);
            for (size_t i = 0; i < old_cap; ++i) newp[i] = e_cache[i];
            for (size_t i = old_cap; i < new_cap; ++i) newp[i] = (seq_t)0;
            seq_free(e_cache);
            e_cache = newp;
            e_cap   = new_cap;
            e_len   = new_cap;
        }
        /* initialise the new e value to zero before accumulation */
        e_cache[a_len] = (seq_t)0;
        /* update all elementary sums in reverse order */
        for (size_t k = a_len; k > 0; --k) {
            e_cache[k] += next_val * e_cache[k - 1];
        }
    }

    return a_cache[(size_t)n];
}
