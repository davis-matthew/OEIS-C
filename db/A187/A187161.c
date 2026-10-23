/*
 * A187161  Number of 8-step one space at a time bishop's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) for n > 9 with initial
 *            values: a(1)=a(2)=a(3)=a(4)=0, a(5)=472, a(6)=3504, a(7)=10576,
 *            a(8)=23340, a(9)=40448. For n > 6 the sequence also satisfies
 *            the quadratic empirical formula a(n) = 2172*n*n - 19816*n +
 *            42860
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    if (n < 1) return 0;                      /* undefined for n<1 */

    size_t N = (size_t)n;                     /* indices are 0‑based in cache */

    /* Ensure capacity for N elements */
    if (N > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < N) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to N */
    if (cache_len < N) {
        /* Fill in the known base terms a(1)..a(9) */
        if (cache_len < 9) {
            static const seq_t base_vals[9] = {0,0,0,0,472,3504,10576,23340,40448};
            size_t upto = N < 9 ? N : 9;
            size_t i;
            for (i = cache_len; i < upto; ++i) {
                cache[i] = base_vals[i];
            }
            cache_len = upto;
        }

        /* Apply the recurrence for n > 9 */
        if (cache_len < N) {
            size_t i;
            for (i = cache_len; i < N; ++i) {
                cache[i] = 3 * cache[i-1] - 3 * cache[i-2] + cache[i-3];
            }
            cache_len = N;
        }
    }

    return cache[n-1];
}
