/*
 * A231971  Number of (n+1) X (2+1) 0..1 arrays with no element having a
 *          strict majority of its horizontal and vertical neighbors equal to
 *          one.
 * https://oeis.org/A231971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A231971: Recurrence implementation
   a(n) = 3*a(n-1) + 2*a(n-3) + 4*a(n-4) - 10*a(n-5) - 2*a(n-6)
          -  a(n-8) + a(n-9)
   Initial values a(1..9):
   16, 56, 169, 550, 1764, 5680, 18225, 58596, 188356
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Base values for indices 1..9 */
    static const seq_t base[9] = {
        16, 56, 169, 550, 1764, 5680, 18225, 58596, 188356
    };

    if (n <= 9) return base[n-1];

    static seq_t *cache = NULL;
    /* highest index that has a computed value */
    static idx_t last = 0;

    if (cache == NULL) {
        /* Allocate space for indices 0..9 (cache[0] unused) */
        cache = (seq_t *)malloc((size_t)10 * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure */
        cache[0] = 0;
        for (idx_t i = 0; i < 9; ++i)
            cache[i + 1] = base[i];
        last = 9;
    }

    if (n > last) {
        /* Ensure the array is large enough */
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        cache = tmp;

        /* Compute missing terms from last+1 up to n */
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t val = (seq_t)3 * cache[i-1]
                      + (seq_t)2 * cache[i-3]
                      + (seq_t)4 * cache[i-4]
                      + cache[i-9];
            val -= (seq_t)10 * cache[i-5]
                  + (seq_t)2 * cache[i-6]
                  + cache[i-8];
            cache[i] = val;
        }
        last = n;
    }

    return cache[n];
}
