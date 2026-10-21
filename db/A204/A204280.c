/*
 * A204280  Number of (n+2) X 6 0..2 arrays with every 3 X 3 subblock having
 *          three equal elements in a row horizontally, vertically, diagonally
 *          or antidiagonally exactly three ways, and new values 0..2
 *          introduced in row major order.
 * https://oeis.org/A204280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 6*a(n-4) + 4*a(n-6) for n > 8 Initial values
 *            (for n = 1 … 8): a(1) = 1758, a(2) = 193, a(3) = 256, a(4) =
 *            244, a(5) = 442, a(6) = 982, a(7) = 1914, a(8) = 4922
 * Range:     u8 none, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is first term, a(0) is undefined */
    if (n == 0)
        return (seq_t)0;

    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t size = 0;                /* current allocated size (>=1) */

    /* Ensure the cache is large enough */
    if (n >= size) {
        idx_t new_size = n + 1;           /* indices 0…n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) {
            /* Out of memory – abort; caller should handle if needed */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        /* Initialize new positions to 0 */
        for (idx_t i = size; i <= n; ++i)
            cache[i] = (seq_t)0;
        size = new_size;
    }

    /* Compute missing terms up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != (seq_t)0)          /* already computed */
            continue;

        /* Initial values for i = 1 … 8 */
        switch (i) {
            case 1: cache[i] = (seq_t)1758; break;
            case 2: cache[i] = (seq_t)193;  break;
            case 3: cache[i] = (seq_t)256;  break;
            case 4: cache[i] = (seq_t)244;  break;
            case 5: cache[i] = (seq_t)442;  break;
            case 6: cache[i] = (seq_t)982;  break;
            case 7: cache[i] = (seq_t)1914; break;
            case 8: cache[i] = (seq_t)4922; break;
            default:
                /* Apply recurrence: a(n) = 3*a(n-1) - 6*a(n-4) + 4*a(n-6) */
                cache[i] = (seq_t)3 * cache[i-1]
                         - (seq_t)6 * cache[i-4]
                         + (seq_t)4 * cache[i-6];
                break;
        }
    }

    return cache[n];
}
