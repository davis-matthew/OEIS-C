/*
 * A131765  Series reversion of x*(1-5x)/(1-x) .
 * https://oeis.org/A131765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   (n+1)*a(n) = 9*(2*n-1)*a(n-1) - (n-2)*a(n-2) Initial values:
 *            a(0)=1, a(1)=4 Recurrence is valid for n ≥ 2. The function
 *            memoises previously computed terms and works for any sequence
 *            type seq_t supplied by the build
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 31,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 200 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static inline seq_t term_from_recurrence(idx_t n, seq_t a_nm1, seq_t a_nm2) {
    /* Compute a(n) = (9*(2*n-1)*a(n-1) - (n-2)*a(n-2)) / (n+1) */
    if (sizeof(seq_t) <= 8) { /* 64‑bit or smaller: use 128‑bit intermediates */
        unsigned __int128 f           = 9;
        unsigned __int128 two_n_minus1  = (unsigned __int128)2 * n - 1;
        unsigned __int128 factor      = f * two_n_minus1;
        unsigned __int128 t1          = (unsigned __int128)a_nm1 * factor;
        unsigned __int128 t2          = (unsigned __int128)a_nm2 * (unsigned __int128)(n - 2);
        unsigned __int128 numer      = t1 - t2;
        return (seq_t)(numer / (n + 1));
    } else { /* arbitrary‑precision seq_t */
        seq_t t1 = 9 * (seq_t)(2 * n - 1) * a_nm1;
        seq_t t2 = (seq_t)(n - 2) * a_nm2;
        seq_t numer = t1 - t2;
        return numer / (n + 1);
    }
}

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated size */

    /* initialise cache with the first two terms */
    if (cap == 0) {
        cap = 2;
        cache = mem_new(seq_t, cap);
        if (!cache) return (seq_t)0;          /* OOM */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
    }

    /* if the requested index is already cached, return it */
    if (n < (idx_t)cap) {
        return cache[(size_t)n];
    }

    /* ensure the cache is large enough */
    size_t old_cap = cap;
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) {
            mem_free(cache);
            return (seq_t)0;      /* OOM */
        }
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = old_cap; i <= n; ++i) {
        seq_t a_nm1 = cache[i - 1];
        seq_t a_nm2 = cache[i - 2];
        cache[i] = term_from_recurrence(i, a_nm1, a_nm2);
    }

    return cache[(size_t)n];
}
