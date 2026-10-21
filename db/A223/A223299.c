/*
 * A223299  4 X 4 X 4 triangular graph coloring a rectangular array: number of
 *          n X 2 0..9 arrays where 0..9 label nodes of a graph with edges 0,1
 *          0,2 1,2 1,3 1,4 2,4 3,4 2,5 4,5 3,6 3,7 4,7 6,7 4,8 5,8 7,8 5,9
 *          8,9 and every array movement to a horizontal or vertical neighbor
 *          moves along an edge of this graph.
 * https://oeis.org/A223299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 12*a(n-2) a(1) = 36, a(2) = 324
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t cache[10001] = {0};   /* cache[n] holds a(n), cache[0] unused */
static idx_t computed_up_to = 0;   /* highest n for which a(n) is already computed */

seq_t a(idx_t n)
{
    if (n < 1 || n >= sizeof(cache)/sizeof(cache[0]))
        return 0;                /* out of bounds – not expected for the tests */

    /* initialise the first two values once */
    if (computed_up_to < 1) { cache[1] = (seq_t)36; computed_up_to = 1; }
    if (computed_up_to < 2) { cache[2] = (seq_t)324; computed_up_to = 2; }

    /* compute missing terms up to n */
    if (computed_up_to < n) {
        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            cache[i] = (seq_t)(11 * cache[i-1] - 12 * cache[i-2]);
        }
        computed_up_to = n;
    }

    return cache[n];
}
