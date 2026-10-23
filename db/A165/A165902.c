/*
 * A165902  a(0)=0, a(1)=1, a(n) = a(n-1) + 3^(n-3)*a(n-2).
 * https://oeis.org/A165902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3^(n-3) * a(n-2), a(0)=0, a(1)=1, a(2)=1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   alloc_seq_array, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */

#include "../utils/alloc_seq_array.h"
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    /* Basic cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;

    /* Memoization state */
    static seq_t *cache   = NULL;   /* cached sequence values */
    static size_t len     = 0;      /* number of valid cache entries */
    static size_t cap     = 0;      /* allocated cache capacity     */
    static seq_t pow3      = 1;     /* 3^(current index-3)          */

    /* One‑time allocation of a small initial table */
    if (!cache) {
        const size_t init_cap = 4; /* at least indices 0…3 will fit */
        cache = alloc_seq_array(init_cap);
        if (!cache) return (seq_t)0;   /* allocation failure – safe fall‑back */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        len      = 3;
        cap      = init_cap;
        pow3     = (seq_t)1;           /* 3^(3-3) = 1 */
    }

    /* Fast path for already cached values */
    if ((size_t)n < len)
        return cache[n];

    /* Grow the cache if necessary */
    if (cap <= (size_t)n) {
        size_t new_cap = (cap == 0) ? 4 : cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        grow_seq_array(&cache, &cap, new_cap);
    }

    /* Compute missing terms up to n */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        seq_t contrib = pow3 * cache[i - 2];
        seq_t term    = cache[i - 1] + contrib;
        cache[i]      = term;
        pow3          = pow3 * (seq_t)3;  /* next power: 3^(i-2) */
    }

    len = (size_t)n + 1; /* cache now contains all terms up to n */
    return cache[n];
}
