/*
 * A223670  Number of 3 X n 0..1 arrays with rows, diagonals and antidiagonals
 *          unimodal.
 * https://oeis.org/A223670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) -21*a(n-2) +35*a(n-3) -35*a(n-4) +21*a(n-5)
 *            -7*a(n-6) +a(n-7) for n>8. Initial values: a(1)=8, a(2)=64,
 *            a(3)=292, a(4)=948, a(5)=2527, a(6)=5913, a(7)=12577, a(8)=24821
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 63, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset is 1; nothing defined for 0 */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;  /* highest n that has been computed */
    static const seq_t init_vals[8] =
        {8, 64, 292, 948, 2527, 5913, 12577, 24821};

    /* grow cache if necessary */
    if (cache_len < n) {
        size_t needed = n + 1;          /* index 0 unused */
        size_t new_cap = cache_cap ? cache_cap : 9;
        while (new_cap < needed) new_cap *= 2;
        if (new_cap != cache_cap) {
            seq_t *tmp = (seq_t*)realloc(cache,
                                         new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* allocation failure */
            cache      = tmp;
            cache_cap  = new_cap;
        }

        /* compute new terms */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 8) {
                cache[i] = init_vals[i - 1];
            } else {
                seq_t pos = 7 * cache[i - 1] +
                            35 * cache[i - 3] +
                            21 * cache[i - 5] +
                            cache[i - 7];
                seq_t neg = 21 * cache[i - 2] +
                            35 * cache[i - 4] +
                            7  * cache[i - 6];
                cache[i] = pos - neg;     /* guaranteed non‑negative */
            }
        }
        cache_len = n;
    }
    return cache[n];
}
