/*
 * A063247  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          100 ).
 * https://oeis.org/A063247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-3) - a(n-5) with initial values a(1)=1,
 *            a(2)=5, a(3)=8, a(4)=11, a(5)=14
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1, return 0 for safety */

    /* cache computed terms; grows as needed */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (cache_size < n) {
        /* allocate enough space */
        idx_t new_size = n;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        /* fill newly allocated entries */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i < 5) {
                static const seq_t init[5] = { 1, 5, 8, 11, 14 };
                cache[i] = init[i];
            } else {
                /* recurrence a(n) = a(n-2)+a(n-3)-a(n-5) */
                cache[i] = cache[i - 2] + cache[i - 3] - cache[i - 5];
            }
        }
        cache_size = new_size;
    }

    return cache[n - 1];  /* array is 0‑based, index n-1 -> a(n) */
}
