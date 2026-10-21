/*
 * A079811  Sum of numbers read upward at a 45-degree angle in A079809.
 * https://oeis.org/A079811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-4) - 2*a(n-5) - a(n-8) + a(n-9) for n > 9
 *            a(1) = 1, a(2) = 2, a(3) = 2, a(4) = 6, a(5) = 7, a(6) = 10,
 *            a(7) = 10, a(8) = 18, a(9) = 19
 * Range:     u8 n <= 35, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle offset (behaviour for n < 1 is irrelevant) */
    if (n == 0)
        return (seq_t)0;

    /* static state for memoisation */
    static seq_t *cache = NULL;   /* indices 0 .. max_computed */
    static size_t cap = 0;        /* current allocated capacity */
    static idx_t computed_max = 0;/* largest n already computed */

    /* ensure we have space for at least n and for all base values up to 9 */
    size_t needed;
    if (n < 9)
        needed = 10;              /* indices 0..9 needed for base values */
    else
        needed = (size_t)n + 1;   /* indices 0 .. n needed */

    if (cap < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp)
            exit(1);              /* allocation failed */
        cache = tmp;
        cap   = needed;
    }

    /* initialise base values once */
    if (computed_max == 0) {
        static const seq_t base[10] = {
            0, /* unused, placeholder for index 0 */
            1, 2, 2, 6, 7, 10, 10, 18, 19
        };
        for (idx_t i = 1; i <= 9; ++i)
            cache[i] = base[i];
        computed_max = 9;
    }

    /* compute any missing terms up to n */
    if (n > computed_max) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            seq_t val = cache[i - 1];
            val += cache[i - 4] * 2;
            val -= cache[i - 5] * 2;
            val -= cache[i - 8];
            val += cache[i - 9];
            cache[i] = val;
        }
        computed_max = n;
    }

    return cache[n];
}
