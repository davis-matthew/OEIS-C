/*
 * A222835  Number of n X 5 0..3 arrays with no element equal to another at a
 *          city block distance of exactly two, and new values 0..3 introduced
 *          in row major order.
 * https://oeis.org/A222835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 20, u64 n <= 45, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A222835
 * Recurrence (for n > 7):
 *   a(n) = 2*a(n-1) + 2*a(n-3) + a(n-4)
 * Initial values:
 *   a(1)=19, a(2)=384, a(3)=648, a(4)=1536,
 *   a(5)=4032, a(6)=9600, a(7)=22848
 *
 * The function memoises results so that calls in any order are fast.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoisation table – cache[i] holds a(i), cache[0] unused */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;   /* number of seq_t elements allocated */
    static idx_t  cache_max = 0;   /* highest index already computed */

    if (n == 0) return 0;          /* undefined for n=0 – return 0 safely */

    /* Ensure the table can hold indices up to n and at least up to 7. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (cache_cap ? cache_cap * 2 : 2);
        while (new_cap <= (size_t)n) new_cap *= 2;
        if (new_cap < 8) new_cap = 8;          /* room for the first 7 terms */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first seven terms once. */
    if (cache_max < 7) {
        cache[1] = 19;
        cache[2] = 384;
        cache[3] = 648;
        cache[4] = 1536;
        cache[5] = 4032;
        cache[6] = 9600;
        cache[7] = 22848;
        cache_max = 7;
    }

    /* Compute any missing terms up to n using the recurrence. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        cache[i] = 2 * cache[i - 1] + 2 * cache[i - 3] + cache[i - 4];
    }
    if (n > cache_max) cache_max = n;

    return cache[n];
}
