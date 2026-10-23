/*
 * A195197  Number of Hamiltonian cycles in the generalized Petersen Graph
 *          P(n,2).
 * https://oeis.org/A195197
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence of order 13 * a(n) =
 *            a(n-2)+a(n-3)+a(n-4)-a(n-5)+a(n-6) *
 *            -a(n-7)-a(n-8)-a(n-9)-a(n-10)+a(n-11)+a(n-13) * with initial
 *            values * a(3)=3, a(4)=8, a(5)=0, a(6)=6, a(7)=7, a(8)=12,
 *            a(9)=3, * a(10)=30, a(11)=0, a(12)=34, a(13)=13, a(14)=56,
 *            a(15)=3
 * Range:     u8 n <= 21, u16 n <= 43, u32 n <= 89, u64 n <= 181,
 *            u128 n <= 365, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoised sequence values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of values currently stored   */
    static idx_t cache_cap = 0;   /* allocated capacity on heap          */

    if (n < 3) {
        return 0; /* undefined for n < 3; harmless to return 0 */
    }

    /* How many terms are needed in the cache (index 0 corresponds to a(3)) */
    idx_t required = n - 3 + 1;

    /* enlarge cache allocation if necessary */
    if (required > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
        while (new_cap < required) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise base terms once */
    if (cache_len == 0) {
        const seq_t base[13] = {3, 8, 0, 6, 7, 12, 3, 30, 0, 34, 13, 56, 3};
        for (idx_t i = 0; i < 13; ++i) cache[i] = base[i];
        cache_len = 13;
    }

    /* extend computation to the required index using the recurrence */
    if (required > cache_len) {
        for (idx_t pos = cache_len; pos < required; ++pos) {
            /* compute a(n) where n = pos + 3 */
            seq_t res = cache[pos - 2];   /* a(n-2) */
            res += cache[pos - 3];        /* + a(n-3) */
            res += cache[pos - 4];        /* + a(n-4) */
            res += cache[pos - 6];        /* + a(n-6) */
            res += cache[pos - 11];       /* + a(n-11) */
            res += cache[pos - 13];       /* + a(n-13) */
            res -= cache[pos - 5];        /* - a(n-5) */
            res -= cache[pos - 7];        /* - a(n-7) */
            res -= cache[pos - 8];        /* - a(n-8) */
            res -= cache[pos - 9];        /* - a(n-9) */
            res -= cache[pos - 10];       /* - a(n-10) */
            cache[pos] = res;
        }
        cache_len = required;
    }

    return cache[n - 3];
}
