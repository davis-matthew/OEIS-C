/*
 * A233214  Number of nX5 0..5 arrays with no element x(i,j) adjacent to value
 *          5-x(i,j) horizontally or antidiagonally, top left element zero,
 *          and 1 appearing before 2 3 and 4, and 2 appearing before 3 in row
 *          major order.
 * https://oeis.org/A233214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1975*a(n-1) - 228357*a(n-2) + 1804281*a(n-3) -
 *            4170978*a(n-4) + 2593080*a(n-5) Empirical recurrence valid for n
 *            ≥ 6 with initial values a(1)=99, a(2)=147019, a(3)=268289572,
 *            a(4)=496475792293, a(5)=919538740854193
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization.  Indices are 1‑based. */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;
    static idx_t computed  = 0;
    static const seq_t init[5] = {
        (seq_t)99ULL,
        (seq_t)147019ULL,
        (seq_t)268289572ULL,
        (seq_t)496475792293ULL,
        (seq_t)919538740854193ULL
    };

    if (n <= 5U) {
        return init[n - 1U];
    }

    /* Ensure the cache can hold term n (index n). */
    if (capacity < n + 1U) {
        idx_t new_cap = capacity ? capacity << 1 : n + 1U;
        if (new_cap < n + 1U)
            new_cap = n + 1U;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;           /* out of memory: return 0 as sentinel */
        cache = tmp;
        capacity = new_cap;
    }

    /* Initialise the first five terms once. */
    if (computed < 5U) {
        for (idx_t i = 1U; i <= 5U; ++i)
            cache[i] = init[i - 1U];
        computed = 5U;
    }

    /* Extend the sequence up to n. */
    for (idx_t i = computed + 1U; i <= n; ++i) {
        cache[i] = cache[i - 1U] * 1975U
                 - cache[i - 2U] * 228357U
                 + cache[i - 3U] * 1804281U
                 - cache[i - 4U] * 4170978U
                 + cache[i - 5U] * 2593080U;
    }

    computed = n;
    return cache[n];
}
