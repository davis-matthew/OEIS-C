/*
 * A086456  Expansion of (1 + x + sqrt(1 - 6*x + x^2))/2 in powers of x.
 * https://oeis.org/A086456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 , a(1) = -1 , a(2) = -2 For n ≥ 3: a(n) = ((6*n -
 *            9)*a(n-1) - (n - 3)*a(n-2)) / n (This follows from n*a(n) +
 *            3*(-2*n + 3)*a(n-1) + (n-3)*a(n-2) = 0)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 14, i64 n <= 22, i128 none,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for malloc/realloc */

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /*----- C++ path --------------------------------------*/
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    if (cache.size() == 0)                    /* initialise base cases */
    {
        cache.resize(3);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)-2;
    }
    if (n >= cache.size())                    /* grow and compute missing terms */
    {
        cache.resize(n + 1);
        for (idx_t i = cache.size() - 1; i > 2; --i)  /* starting at old_size */
        {
            seq_t i_seq = (seq_t)(i);
            seq_t numerator =
                ((seq_t)6 * i_seq - (seq_t)9) * cache[i - 1]
                - (i_seq - (seq_t)3) * cache[i - 2];
            cache[i] = numerator / i_seq;     /* division is exact */
        }
    }
    return cache[n];

#else
    /*----- C path ----------------------------------------*/
    static seq_t *cache = NULL;        /* allocated cache */
    static idx_t cap   = 0;            /* capacity of cache   */
    static idx_t len   = 0;            /* number of initialised elements */

    /* Ensure capacity for the required indices */
    if (n + 1 > cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failed – return 0 */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise base cases once */
    if (len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)-2;
        len = 3;
    }

    /* Compute missing values up to n */
    if (n >= len) {
        for (idx_t i = len; i <= n; ++i) {
            seq_t i_seq = (seq_t)i;
            seq_t part1 = ((seq_t)6 * i_seq - (seq_t)9) * cache[i - 1];
            seq_t part2 = (i_seq - (seq_t)3) * cache[i - 2];
            cache[i] = (part1 - part2) / i_seq;   /* division is exact */
        }
        len = n + 1;
    }

    return cache[n];
#endif
}
