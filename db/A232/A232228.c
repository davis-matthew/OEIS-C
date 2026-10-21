/*
 * A232228  a(1)=1; thereafter a(n) = 2^(number of bits in binary expansion of
 *          a(n-1)) + 1 + a(n-1).
 * https://oeis.org/A232228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   bitlen_seq_t, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, thereafter a(n)=2^(bitlen(a(n-1))) + 1 + a(n-1). */
#include <stdlib.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Undefined behaviour for n < 1; return 0 as a safe fallback. */
    if (n < 1)
        return (seq_t)0;

    /* Lazy static cache: 1‑based array of computed terms */
    static seq_t *cache   = NULL;  /* allocated memory for the cache    */
    static idx_t cache_cap = 0;    /* number of slots currently allocated */
    static idx_t cache_len = 0;    /* highest index that has been computed */

    /* Grow the cache if it doesn't hold a[n] yet */
    if (cache_cap <= n) {
        idx_t new_cap = (cache_cap == 0) ? (n + 1) : (cache_cap * 2);
        if (new_cap <= n) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                 /* allocation failure is fatal */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first term if not yet stored */
    if (cache_len == 0) {
        cache[1] = (seq_t)1;
        cache_len = 1;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        unsigned lenbits = bitlen_seq_t(prev);
        seq_t addend = pow2((idx_t)lenbits);
        cache[i] = prev + addend + (seq_t)1;
    }
    cache_len = n;

    return cache[n];
}
