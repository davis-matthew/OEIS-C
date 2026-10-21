/*
 * A079825  Sum of numbers in n-th upward diagonal of triangle in A079826.
 * https://oeis.org/A079825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Recurrence derived from G.f. a(n) =
 *            a(n-1)+a(n-2)-a(n-3)+2*a(n-4)-2*a(n-5)-2*a(n-6)+2*a(n-7)
 *            -a(n-8)+a(n-9)+a(n-10)-a(n-11), n≥12 with initial values:
 *            a(1)=1, a(2)=3, a(3)=6, a(4)=15, a(5)=26, a(6)=41, a(7)=62,
 *            a(8)=92, a(9)=129, a(10)=169, a(11)=224
 * Range:     u8 n <= 11, u16 n <= 76, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t computed_up_to = 0;

    /* initial values for n=1..11 */
    static const seq_t init[12] = {
        0,                      /* cache[0] unused */
        1, 3, 6, 15, 26, 41, 62, 92, 129, 169, 224
    };

    /* allocate initial cache and fill first 11 terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 12);
        for (idx_t i = 1; i <= 11; ++i)
            cache[i] = init[i];
        computed_up_to = 11;
    }

    /* return if already computed */
    if (n <= computed_up_to)
        return cache[n];

    /* grow cache to accommodate new n */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    if (!new_cache) return 0;   /* allocation failed, return 0 */
    cache = new_cache;

    /* compute values from computed_up_to + 1 up to n */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        /* use signed 64‑bit intermediate. The values for n≤10000 stay
           within 64‑bit signed range, so this is safe. */
        long long val =
            (long long)cache[i-1] + (long long)cache[i-2]
          - (long long)cache[i-3] + 2LL * (long long)cache[i-4]
          - 2LL * (long long)cache[i-5] - 2LL * (long long)cache[i-6]
          + 2LL * (long long)cache[i-7] - (long long)cache[i-8]
          + (long long)cache[i-9] + (long long)cache[i-10]
          - (long long)cache[i-11];

        cache[i] = (seq_t)val;
    }
    computed_up_to = n;
    return cache[n];
}
