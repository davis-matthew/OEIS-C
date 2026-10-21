/*
 * A216317  G.f.: 1/( (1-8*x)*(1+x)^2 )^(1/6).
 * https://oeis.org/A216317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A216317 : G.f.: 1/( (1-8*x)*(1+x)^2 )^(1/6) */
/* Recurrence: n*a(n) = (7*n-6)*a(n-1) + 4*(2*n-3)*a(n-2)  (n≥2)   */
/* Initial values: a(0)=1, a(1)=1 */

#include <stddef.h>   /* size_t, NULL */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc, free */
#include "../utils/alloc_seq.h"   /* alloc_seq(size_t) -> seq_t* */
#include "../utils/free_seq.h"    /* free_seq(seq_t*) */

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms */
    static seq_t *cache   = NULL;
    static idx_t  cap     = 0;          /* allocated capacity (number of terms) */
    static idx_t  last_np = 0;          /* largest index that has been computed */

    /* Expand cache if necessary */
    if (n + 1 > cap) {
        idx_t new_cap = (cap ? cap * 2 : 2);
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *new_cache = alloc_seq(new_cap);
        if (cache) {
            for (idx_t i = 0; i < cap; ++i) new_cache[i] = cache[i];
            free_seq(cache);
        }
        cache = new_cache;
        cap   = new_cap;
    }

    /* Initialise base cases if not already done */
    if (last_np == 0) {
        cache[0] = (seq_t)1;
        if (cap > 1) cache[1] = (seq_t)1;
        last_np = (n >= 1 ? 1 : 0);
    }

    /* Return already known values */
    if (n == 0) return cache[0];
    if (n == 1) return cache[1];

    /* Compute terms from last_np+1 up to n.
       For C (seq_t is an integer) use 128‑bit arithmetic to avoid overflow.
       For C++ (seq_t may be an arbitrary‑precision class) use ordinary
       multiplication/division operators.                                     */
    for (idx_t i = last_np + 1; i <= n; ++i) {
#ifdef __cplusplus
        seq_t term1 = (seq_t)(7 * i - 6) * cache[i - 1];
        seq_t term2 = (seq_t)4 * (seq_t)(2 * i - 3) * cache[i - 2];
        seq_t numer = term1 + term2;
        cache[i] = numer / (seq_t)i;
#else
        unsigned __int128 t1 = (unsigned __int128)(7 * i - 6) * cache[i - 1];
        unsigned __int128 t2 = (unsigned __int128)4 * (2 * i - 3) * cache[i - 2];
        unsigned __int128 numer = t1 + t2;
        cache[i] = (seq_t)(numer / i);
#endif
    }

    last_np = n;
    return cache[n];
}
