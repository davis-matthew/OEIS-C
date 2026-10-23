/*
 * A036113  A summarize Fibonacci sequence starting with a(0)=3 and a(1)=2:
 *          summarize the previous two terms!.
 * https://oeis.org/A036113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   append_count_digit, digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0)=3, a(1)=2
 * Recurrence: for n≥2, let a(n) be the concatenation, for each digit d from 9 down to 0,
 *             of the decimal representation of the number of occurrences of d in a(n‑2) and a(n‑1),
 *             followed by the digit d itself, only when the count is positive.
 */
#include <stdlib.h>
#include "../utils/digit_counts.h"
#include "../utils/append_count_digit.h"

seq_t a(idx_t n)
{
    /* Static memoisation cache. */
    static seq_t *cache   = NULL;   /* array of terms */
    static idx_t  cap     = 0;      /* allocated capacity */
    static idx_t  computed = 1;     /* index of last computed term */

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)2;
        computed = 1;
    }

    /* If requested term already computed, return it. */
    if (n <= computed)
        return cache[n];

    /* Grow cache if necessary. */
    if (cap <= n) {
        size_t new_cap = cap > 0 ? cap * 2 : 2;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        unsigned char c1[10] = {0}, c2[10] = {0};
        digit_counts(cache[i - 1], c1);
        digit_counts(cache[i - 2], c2);
        seq_t result = 0;
        for (int d = 9; d >= 0; --d) {
            seq_t cnt = (seq_t)c1[d] + (seq_t)c2[d];
            if (cnt > 0) {
                append_count_digit(&result, cnt, d);
            }
        }
        cache[i] = result;
    }
    computed = n;
    return cache[n];
}
