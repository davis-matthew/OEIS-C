/*
 * A165880  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A165880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*(a(n-1)+a(n-2)+...+a(n-9)) - 136*a(n-10) for n >= 11,
 *            with a(0)=1, a(1)=18, a(2)=306, a(3)=5202, a(4)=88434,
 *            a(5)=1503378, a(6)=25557426, a(7)=434476242, a(8)=7386096114,
 *            a(9)=125563633938, a(10)=2134581776793
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for computed terms.  cache[i] holds a(i). */
    static seq_t *cache = NULL;
    static idx_t cap      = 0;
    static idx_t computed = 0;  /* number of cached terms   */

    /* Base values for indices 0..10. */
    static const seq_t base[11] = {
        1ULL, 18ULL, 306ULL, 5202ULL, 88434ULL,
        1503378ULL, 25557426ULL, 434476242ULL,
        7386096114ULL, 125563633938ULL, 2134581776793ULL
    };

    /* For n <= 10 we can return the known value directly. */
    if (n <= 10)
        return base[n];

    /* On the first call allocate the cache and copy the base values. */
    if (!cache) {
        cap = n + 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;           /* allocation failure */

        for (idx_t i = 0; i <= 10; ++i)
            cache[i] = base[i];
        computed = 11;                  /* indices 0..10 are ready */
    }

    /* If a later call requests a larger index, grow the cache. */
    if (n >= cap) {
        idx_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        cap = new_cap;
    }

    /* Compute all missing terms up to n (if any). */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t j = 1; j <= 9; ++j)
            sum += cache[i - j];
        cache[i] = 16 * sum - 136 * cache[i - 10];
    }
    computed = n + 1;                 /* we now have values up to n */

    return cache[n];
}
