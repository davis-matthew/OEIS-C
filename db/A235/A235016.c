/*
 * A235016  Number of (n+1) X (7+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 11400, a(2) = 14600, a(3) = 19560, a(4) = 28968, a(5) =
 *            44680, a(6) = 74888, a(7) = 128776, a(8) = 235240, a(9) =
 *            437928, a(10) = 853928 a(n) = 5*a(n‑1) + a(n‑2) – 38*a(n‑3) +
 *            33*a(n‑4) + 97*a(n‑5) – 127*a(n‑6) – 88*a(n‑7) + 154*a(n‑8) +
 *            12*a(n‑9) – 48*a(n‑10), for n ≥ 11
 * Range:     u8 none, u16 n <= 5, u32 n <= 20, u64 n <= 44, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization (1‑based index) */
    static seq_t *cache      = NULL;   /* allocates n+1 entries for n≥1 */
    static idx_t  cache_cap  = 0;      /* capacity of cache array (in entries) */
    static idx_t  cache_last = 0;      /* largest n that has been computed */

    /* initial values (first 10 terms) */
    static const seq_t init[10] = {
        (seq_t)11400, (seq_t)14600, (seq_t)19560, (seq_t)28968,
        (seq_t)44680, (seq_t)74888, (seq_t)128776, (seq_t)235240,
        (seq_t)437928, (seq_t)853928
    };

    if (n == 0) return 0;          /* offset is 1 */

    /* ensure cache is large enough */
    if (cache_cap < n + 1) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;        /* allocation failure – return 0 as sentinel */
        cache = tmp;
        cache_cap = n + 1;
    }

    /* compute values up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i <= 10) {
            cache[i] = init[i - 1];
        } else {
            seq_t v = 0;
            v += cache[i - 1] * 5;
            v += cache[i - 2];
            v -= cache[i - 3] * 38;
            v += cache[i - 4] * 33;
            v += cache[i - 5] * 97;
            v -= cache[i - 6] * 127;
            v -= cache[i - 7] * 88;
            v += cache[i - 8] * 154;
            v += cache[i - 9] * 12;
            v -= cache[i -10] * 48;
            cache[i] = v;
        }
    }
    cache_last = n;

    return cache[n];
}
