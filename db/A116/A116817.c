/*
 * A116817  Number of permutations of length n which avoid the patterns 2341,
 *          3241, 4132.
 * https://oeis.org/A116817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=74 a(n) = 6*a(n-1) -
 *            10*a(n-2) + 3*a(n-3) + 3*a(n-4) - a(n-5) for n>=6
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;/* cache[i] holds a(i) for i>=1, 0 unused */
static idx_t max_computed = 0;/* largest n for which cache[n] is valid */

static const seq_t BASE[6] = { (seq_t)0, (seq_t)1, (seq_t)2, (seq_t)6,
                               (seq_t)21, (seq_t)74 };

seq_t a(idx_t n)
{
    if (n <= 5)
        return BASE[n];

    if (cache == NULL || n > max_computed) {
        size_t new_size = (size_t)n + 1;        /* indices 0 .. n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;                    /* allocation failed */
        cache = tmp;

        /* initialise base values if not already present */
        if (max_computed < 5) {
            for (idx_t i = 1; i <= 5 && i <= n; ++i)
                cache[i] = BASE[i];
        }

        /* compute missing terms up to n */
        idx_t start = max_computed + 1;
        if (start < 6) start = 6;
        for (idx_t i = start; i <= n; ++i) {
            cache[i] = 6 * cache[i-1] - 10 * cache[i-2]
                       + 3 * cache[i-3] + 3 * cache[i-4] - cache[i-5];
        }

        max_computed = n;
    }
    return cache[n];
}
