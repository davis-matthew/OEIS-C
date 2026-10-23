/*
 * A165651  Number of reduced words of length n in Coxeter group on 36
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A165651 – Number of reduced words of length n in the Coxeter
 * group on 36 generators with relations S_i^2 = (S_i S_j)^9 = I.
 *
 * The generating function is 
 *   1 + 2t + 2t^2 + … + 2t^8 + t^9
 *   ----------------------------------
 *   1 - 34t - 34t^2 - … - 34t^8 + 595t^9
 *
 * Hence, for n ≥ 9 the sequence satisfies the linear recurrence
 *   a(n) = 34·(a(n‑1) + a(n‑2) + … + a(n‑8)) – 595·a(n‑9)
 * with the initial values
 *   a(0)=1, a(1)=36, a(2)=1260, a(3)=44100, a(4)=1543500,
 *   a(5)=54022500, a(6)=1890787500, a(7)=66177562500,
 *   a(8)=2316214687500.
 *
 * The recurrence holds for n ≥ 10; for n = 9 the generating function
 * contributes an extra +1, so
 *   a(9) = 34·(a(8)+…+a(1)) – 595·a(0) + 1.
 *
 * The function below uses a lazy static cache; any call with any n
 * will compute all values up to that n only once and return the result.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;   /* number of elements currently stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* base values for 0 ≤ i ≤ 8 */
    static const seq_t base[9] = {
        (seq_t)1ULL,
        (seq_t)36ULL,
        (seq_t)1260ULL,
        (seq_t)44100ULL,
        (seq_t)1543500ULL,
        (seq_t)54022500ULL,
        (seq_t)1890787500ULL,
        (seq_t)66177562500ULL,
        (seq_t)2316214687500ULL
    };

    /* enlarge the cache if necessary */
    if ((idx_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 9) {                /* fill in the first nine terms */
            cache[i] = base[i];
        } else {                    /* recurrence for n ≥ 9 */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 8; ++k)
                sum += cache[i - k];
            seq_t val = sum * (seq_t)34 - (seq_t)595 * cache[i - 9];
            if (i == 9)              /* the special +1 constant for n = 9 */
                val += (seq_t)1;
            cache[i] = val;
        }
    }

    cache_len = n + 1;
    return cache[n];
}
