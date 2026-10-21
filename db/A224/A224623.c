/*
 * A224623  Number of (n+4) X 10 0..2 matrices with each 5 X 5 subblock
 *          idempotent.
 * https://oeis.org/A224623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4) + a(n-5) * -
 *            3*a(n-6) + 3*a(n-7) - a(n-8) for n > 12 * Initial values for n =
 *            1 … 12 are hard‑coded
 * Range:     u8 none, u16 n <= 8, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* The first 12 terms of the sequence. */
static const seq_t init_vals[12] = {
    (seq_t)43489, (seq_t)24252, (seq_t)28949, (seq_t)31623,
    (seq_t)33115, (seq_t)34032, (seq_t)42214, (seq_t)55189,
    (seq_t)70669, (seq_t)87402, (seq_t)104808, (seq_t)130151
};

/* Compute one term from the recurrence.  Uses the sequence
   values already stored in c[1…i-1] . */
static inline seq_t compute_term(const seq_t *c, idx_t i)
{
#if defined(__INT128__) || defined(__SIZEOF_INT128__)
    __int128 t = 0;
    t += (__int128)4 * (__int128)c[i - 1];
    t -= (__int128)6 * (__int128)c[i - 2];
    t += (__int128)4 * (__int128)c[i - 3];
    t -= (__int128)1 * (__int128)c[i - 4];
    t += (__int128)1 * (__int128)c[i - 5];
    t -= (__int128)3 * (__int128)c[i - 6];
    t += (__int128)3 * (__int128)c[i - 7];
    t -= (__int128)1 * (__int128)c[i - 8];
    return (seq_t)t;
#else
    /* Fallback for types without 128‑bit support.  All values fit
       in the available type and the final result is non‑negative. */
    seq_t pos = c[i - 1] * 4 + c[i - 3] * 4 + c[i - 5] + c[i - 7] * 3;
    seq_t neg = c[i - 2] * 6 + c[i - 4] + c[i - 6] * 3 + c[i - 8];
    return pos - neg;
#endif
}

/* Dynamically ensure that *arr has at least 'needed' elements. */
static void ensure_capacity(size_t needed, seq_t **arr, size_t *cap)
{
    if (*cap >= needed)
        return;

    size_t new_cap = (*cap == 0 ? 1 : *cap);
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = seq_alloc_new_array(new_cap);
    if (*arr) {
        for (size_t i = 0; i < *cap; ++i)
            tmp[i] = (*arr)[i];
        seq_free_array(*arr);
    }
    *arr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size (>= n+1) */
    static idx_t max    = 0;   /* largest index computed so far */

    /* Initialise cache on first call. */
    if (!cache) {
        cap = 13;                 /* indices 0…12 */
        cache = seq_alloc_new_array(cap);
        for (idx_t i = 1; i <= 12; ++i)
            cache[i] = init_vals[i - 1];
        max = 12;
    }

    /* Grow cache if necessary. */
    ensure_capacity((size_t)n + 1, &cache, &cap);

    /* Compute missing terms up to n. */
    for (idx_t i = max + 1; i <= n; ++i) {
        if (i <= 12)
            cache[i] = init_vals[i - 1];  /* safety, though already set */
        else
            cache[i] = compute_term(cache, i);
    }
    max = n;
    return cache[n];
}
