/*
 * A218199  Hilltop maps: number of n X 1 binary arrays indicating the
 *          locations of corresponding elements not exceeded by any horizontal
 *          or vertical neighbor in a random 0..2 n X 1 array.
 * https://oeis.org/A218199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) + a(n-5) Initial values
 *            (OEIS offset 1): a(1)=1, a(2)=3, a(3)=7, a(4)=13, a(5)=25
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 65, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) is outside the offset; return 0 for consistency */
    if (n == 0) return (seq_t)0;

    /* static cache: indices 0..capacity-1
       cache[0] unused, cache[1]..cache[max_n] are valid */
    static seq_t *cache = NULL;
    static size_t capacity = 0;          /* allocated memory size */
    static idx_t  max_n = 0;             /* highest n computed */

    /* required capacity (include index n) */
    size_t need = (size_t)n + 1;
    if (need < 6) need = 6;              /* need space for indices 1..5 */

    /* allocate or grow cache */
    if (cache == NULL) {
        capacity = need;
        cache = (seq_t*)malloc(sizeof(seq_t) * capacity);
        if (cache == NULL) exit(EXIT_FAILURE);

        /* initialize first five terms */
        cache[0] = (seq_t)0;          /* unused sentinel */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)7;
        cache[4] = (seq_t)13;
        cache[5] = (seq_t)25;

        max_n = 5;
    }
    else if (need > capacity) {
        size_t new_cap = need;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        capacity = new_cap;
    }

    /* compute any missing terms up to n */
    if (n > max_n) {
        /* careful with unsigned indices in loop */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            cache[i] =
                cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4] + cache[i-5];
        }
        max_n = n;
    }

    return cache[n];
}
