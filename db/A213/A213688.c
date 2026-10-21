/*
 * A213688  a(n) = Sum_{i=0..n} A000129(i)^3.
 * https://oeis.org/A213688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=0..n} Pell(i)^3, with Pell numbers: P_0 = 0, P_1 =
 *            1, P_n = 2*P_{n-1} + P_{n-2}. The sequence satisfies the linear
 *            recurrence a_n = 13 a_{n-1} + 18 a_{n-2} - 42 a_{n-3} + 11
 *            a_{n-4} + a_{n-5}, with initial values a_0=0, a_1=1, a_2=9,
 *            a_3=134, a_4=1862. The function memoizes computed...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of valid entries in cache */

    /* initialise cache with the first five terms */
    if (cache_len == 0) {
        idx_t need = 5;                  /* indices 0..4 */
        cache = (seq_t *)malloc(need * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 9;
        cache[3] = 134;
        cache[4] = 1862;
        cache_len = 5;
    }

    /* grow cache if needed */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        if (new_len < 5) new_len = 5;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

        /* compute terms from cache_len up to n using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t term = cache[i - 1] * 13;          /* 13*a_{n-1} */
            term += cache[i - 2] * 18;              /* +18*a_{n-2} */
            term += cache[i - 4] * 11;              /* +11*a_{n-4} */
            term += cache[i - 5];                  /* +  a_{n-5} */
            term -= cache[i - 3] * 42;              /* -42*a_{n-3} */
            cache[i] = term;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
