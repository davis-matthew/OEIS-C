/*
 * A028220  Expansion of 1/((1-7x)(1-8x)(1-9x)(1-11x)).
 * https://oeis.org/A028220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (11^(n+3)-6*9^(n+3)+8^(n+4)-3*7^(n+3))/24
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 35*a(n‑1) – 455*a(n‑2) + 2605*a(n‑3) – 5544*a(n‑4)  for n ≥ 4 */
/* with a(0)=1, a(1)=35, a(2)=770, a(3)=13630 */

#include <stddef.h>
#include <stdlib.h>

/* Computes the n-th term of OEIS A028220  */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cached_max = (idx_t)-1;   /* not yet initialised */

    /* Helper to grow the cache up to needed size */
    if (cache == NULL) {
        /* Allocate initial capacity for indices 0..3 */
        cache = (seq_t *)malloc(sizeof(seq_t) * 4);
        if (!cache) return 0; /* out of memory; behaviour undefined */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)35;
        cache[2] = (seq_t)770;
        cache[3] = (seq_t)13630;
        cached_max = 3;
    }

    if (n <= cached_max) {
        return cache[n];
    }

    /* Ensure capacity */
    idx_t new_cap = n + 1;
    if ((size_t)new_cap > (size_t)(-1) / sizeof(seq_t)) {
        /* Prevent overflow of size_t when allocating very large arrays */
        return 0;
    }
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)new_cap);
    if (!tmp) return 0; /* allocation failure */
    cache = tmp;

    /* Constants for recurrence */
    const seq_t C1 = (seq_t)35;
    const seq_t C2 = (seq_t)455;
    const seq_t C3 = (seq_t)2605;
    const seq_t C4 = (seq_t)5544;

    /* Compute terms up to n using recurrence */
    for (idx_t k = cached_max + 1; k <= n; ++k) {
        seq_t t1 = cache[k-1] * C1;
        seq_t t2 = cache[k-2] * C2;
        seq_t t3 = cache[k-3] * C3;
        seq_t t4 = cache[k-4] * C4;
        seq_t val = t1 - t2;
        val = val + t3;
        val = val - t4;
        cache[k] = val;
    }

    cached_max = n;
    return cache[n];
}
