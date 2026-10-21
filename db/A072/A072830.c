/*
 * A072830  Absolute value of 2*b(n)-9*n, where b(n) = accumulative sum of the
 *          greatest digit of n minus the least digit of n (A037904).
 * https://oeis.org/A072830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = |2*b(n) – 9*n| where b(n) = Σ_{k=1..n} (max digit of k –
 *            min digit of k) (offset 1)
 * Range:     u8 n <= 28, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   digit_diff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_diff.h"

seq_t a(idx_t n)
{
    /* static cache of computed values of a(n) and auxiliary state */
    static seq_t *cache = NULL;
    static idx_t cache_sz = 0;   /* number of allocated entries */
    static idx_t computed_max = 0; /* highest n for which a(n) is cached */
    static seq_t sum_b = 0;      /* Σ_{k=1..computed_max} (mx-ls) */

    if (n == 0) return (seq_t)0;   /* undefined for offset‑0, return 0 */

    /* enlarge cache if necessary */
    if (n >= cache_sz) {
        idx_t new_sz = (cache_sz ? cache_sz << 1 : 64);
        while (new_sz <= n) new_sz <<= 1;
        seq_t *new_cache = (seq_t*)malloc(new_sz * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* allocation failure -> 0 */
        for (idx_t i = 0; i < cache_sz; ++i) new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_sz = new_sz;
    }

    /* compute values up to n */
    for (idx_t k = computed_max + 1; k <= n; ++k) {
        sum_b += digit_diff(k);
        seq_t twob = sum_b << 1;              /* 2 * sum_b */
        seq_t nin9 = 9 * (seq_t)k;            /* 9 * n */
        seq_t val = (twob >= nin9) ? (twob - nin9) : (nin9 - twob);  /* |2*b - 9*n| */
        cache[k] = val;
    }
    computed_max = n;

    return cache[n];
}
