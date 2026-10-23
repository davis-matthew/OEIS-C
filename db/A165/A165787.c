/*
 * A165787  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A165787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7∑_{k=1}^{9}a(n−k) − 28a(n−10) for n ≥ 11 with initial
 *            values a(0)=1, a(1)=9, a(2)=72, a(3)=576, a(4)=4608, a(5)=36864,
 *            a(6)=294912, a(7)=2359296, a(8)=18874368, a(9)=150994944, and
 *            a(10)=1207959516
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static state for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t size   = 0;   /* number of values currently stored */

    /* explicit initial values up to n = 9 */
    static const seq_t base[10] = {
        (seq_t)1ULL, (seq_t)9ULL, (seq_t)72ULL, (seq_t)576ULL,
        (seq_t)4608ULL, (seq_t)36864ULL, (seq_t)294912ULL,
        (seq_t)2359296ULL, (seq_t)18874368ULL, (seq_t)150994944ULL
    };

    /* ensure the cache can hold indices up to n */
    if ((idx_t)cap <= n) {
        size_t new_cap = cap ? cap * 2 : (size_t)(n + 1);
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failed – unlikely in tests */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute values up to n (if not already cached) */
    for (idx_t i = size; i <= n; ++i) {
        if (i < 10) {
            cache[i] = base[(size_t)i];
        } else if (i == 10) {
            seq_t val = 0;
            for (idx_t k = 1; k <= 9; ++k)
                val += (seq_t)7 * cache[i - k];
            val -= (seq_t)28 * cache[i - 10];
            val += (seq_t)1;         /* N(t) coefficient for t¹⁰ */
            cache[i] = val;
        } else { /* i ≥ 11, N(t) coefficient is 0 */
            seq_t val = 0;
            for (idx_t k = 1; k <= 9; ++k)
                val += (seq_t)7 * cache[i - k];
            val -= (seq_t)28 * cache[i - 10];
            cache[i] = val;
        }
    }
    size = n + 1;
    return cache[n];
}
