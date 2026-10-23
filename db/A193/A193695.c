/*
 * A193695  Number of arrays of -1..1 integers x(1..n) with every x(i) in a
 *          subsequence of length 1, 2 or 3 with sum zero.
 * https://oeis.org/A193695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + a(n-4) + a(n-5) (n ≥ 6) * Initial
 *            values: a(1)=1, a(2)=3, a(3)=9, a(4)=23, a(5)=57
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;          /* cache[0] unused, cache[1] = a(1), ... */
    static size_t cache_computed = 0;    /* indices 0..cache_computed-1 are filled */

    if (n == (idx_t)0) /* out‑of‑range, return 0 */
        return (seq_t)0;

    /* ensure cache large enough for index n */
    if (n >= cache_computed) {
        size_t new_size = (size_t)n + 1; /* allocate indices 0..n */

        if (cache == NULL) {
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
            if (cache == NULL) return (seq_t)0; /* allocation failed */
            cache[0] = (seq_t)0;               /* unused slot */
        } else if (new_size > cache_computed) {
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0; /* allocation failed */
            cache = tmp;
        }

        /* compute missing values */
        for (idx_t i = cache_computed; i <= n; ++i) {
            seq_t val;
            if (i == 1)          val = (seq_t)1;
            else if (i == 2)     val = (seq_t)3;
            else if (i == 3)     val = (seq_t)9;
            else if (i == 4)     val = (seq_t)23;
            else if (i == 5)     val = (seq_t)57;
            else                 /* i ≥ 6 */
                val = (2 * cache[i-1]) + cache[i-2] + cache[i-4] + cache[i-5];
            cache[i] = val;
        }
        cache_computed = new_size;
    }

    return cache[(size_t)n];
}
