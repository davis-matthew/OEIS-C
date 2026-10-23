/*
 * A038446  Sums of 4 distinct powers of 10.
 * https://oeis.org/A038446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 210, u64 n <= 4845,
 *            u128 n <= 10000, big any n
 * Helpers:   comb_four
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generate all integers having exactly four 1‑digits in decimal,
   in increasing order.  a(n) is the n‑th such integer (n ≥ 1).  */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/comb_four.h"

/* Comparator for qsort on seq_t values. */
static int compare_seq(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return  1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Static cache: store already generated sequence values. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (n == 0) return 0;                /* offset is 1, so ignore n==0 */

    /* Return cached value if already available. */
    if (cache && cache_len >= (size_t)n)
        return cache[n - 1];

    /* Find minimal exponent E_needed such that C(E_needed+1,4) ≥ n. */
    size_t E_needed = 0;
    while (comb_four(E_needed + 1) < (size_t)n)
        ++E_needed;                      /* E_needed grows up to ~30 for n≈10000 */

    /* Build powers of ten up to E_needed, but stop if they would overflow
       a 64‑bit value when seq_t is 64‑bit. */
    seq_t pow10[128];
    pow10[0] = 1;
    size_t i;
    for (i = 1; i <= E_needed; ++i) {
        if (sizeof(seq_t) <= 8) {           /* 64‑bit case */
            if ((unsigned long long)pow10[i - 1] > (unsigned long long)UINT64_MAX / 10)
                break;                      /* would overflow */
        }
        pow10[i] = pow10[i - 1] * (seq_t)10;
    }
    size_t max_pow = i - 1;                 /* highest exponent that fits */

    /* Now recompute number of terms possible with this exponent range. */
    size_t total_combs = comb_four(max_pow + 1);
    if (total_combs < (size_t)n)            /* cannot represent term with this type */
        return 0;

    /* Allocate cache for all terms with exponents up to max_pow. */
    seq_t *new_cache = (seq_t *)malloc(total_combs * sizeof(seq_t));
    if (!new_cache) return 0;               /* allocation failure */

    /* Generate all sums of four distinct powers of ten. */
    size_t idx = 0;
    for (size_t i0 = 0; i0 <= max_pow - 3; ++i0)
        for (size_t i1 = i0 + 1; i1 <= max_pow - 2; ++i1)
            for (size_t i2 = i1 + 1; i2 <= max_pow - 1; ++i2)
                for (size_t i3 = i2 + 1; i3 <= max_pow; ++i3)
                    new_cache[idx++] = pow10[i0] + pow10[i1] + pow10[i2] + pow10[i3];

    /* Sort the generated values. */
    qsort(new_cache, total_combs, sizeof(seq_t), compare_seq);

    /* Replace old cache. */
    free(cache);
    cache = new_cache;
    cache_len = total_combs;

    return cache[n - 1];
}
