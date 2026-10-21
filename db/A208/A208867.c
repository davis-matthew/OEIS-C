/*
 * A208867  Number of n X 3 0..3 arrays with new values 0..3 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A208867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 55*a(n-1) - 261*a(n-2) + 369*a(n-3) - 162*a(n-4) (n>5)
 *            Initial values: a(1)=5, a(2)=136, a(3)=6520, a(4)=324682,
 *            a(5)=16205218
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* number of allocated elements (0..len-1) */

    if (n == 0U) return 0;                /* undefined for n=0, behaviour does not matter */

    /* Resize cache if necessary */
    size_t target = (size_t)n + 1;        /* we need indices 0..n inclusive */
    if (cache_len < target) {
        seq_t *tmp = (seq_t *)realloc(cache, target * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failed – return zero (unlikely) */
        cache = tmp;

        /* Initialize new entries */
        for (size_t i = cache_len; i < target; ++i) {
            switch (i) {
                case 0: /* unused, keep as 0 */
                    cache[i] = 0;
                    break;
                case 1:
                    cache[i] = 5;
                    break;
                case 2:
                    cache[i] = 136;
                    break;
                case 3:
                    cache[i] = 6520;
                    break;
                case 4:
                    cache[i] = 324682;
                    break;
                case 5:
                    cache[i] = 16205218;
                    break;
                default: {
                    seq_t result = cache[i-1] * (seq_t)55;
                    result += cache[i-3] * (seq_t)369;
                    result -= cache[i-2] * (seq_t)261;
                    result -= cache[i-4] * (seq_t)162;
                    cache[i] = result;
                }
            }
        }
        cache_len = target;
    }

    return cache[n];
}
