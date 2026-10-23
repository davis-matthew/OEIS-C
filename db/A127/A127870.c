/*
 * A127870  Number of tilings of a 4 X n board with 1 X 1 and L-shaped tiles
 *          (where the L-shaped tiles cover 3 squares).
 * https://oeis.org/A127870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1,a(1)=1,a(2)=33,a(3)=195,a(4)=2023,a(5)=16839,
 *            a(6)=151817,a(7)=1328849,a(8)=11758369,
 *            a(n)=5a(n−1)+34a(n−2)+6a(n−3)−72a(n−4)−28a(n−5)+74a(n−6)
 *            −10a(n−7)−4a(n−8)−4a(n−9) for n≥9
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[9] = {
    (seq_t)1, (seq_t)1, (seq_t)33, (seq_t)195, (seq_t)2023,
    (seq_t)16839, (seq_t)151817, (seq_t)1328849, (seq_t)11758369
};

static seq_t *cache = NULL;   /* cache of previously computed terms */
static size_t cap   = 0;      /* current allocated size of the cache */
static idx_t last   = (idx_t)-1;  /* index of the highest term computed */

/* Return a(n) for the 4×n tiling problem. */
seq_t a(idx_t n)
{
    /* For the first nine indices we can return the constant table. */
    if (n < 9) return init[n];

    /* Ensure the cache has enough space. */
    size_t needed = (size_t)(n + 1);
    if (cap < needed) {
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        cap   = needed;
    }

    /* Initialise the first nine elements only once. */
    if (last == (idx_t)-1) {
        for (idx_t i = 0; i < 9; ++i) cache[i] = init[i];
        last = 8;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] =   5 * cache[i-1]
                   + 34 * cache[i-2]
                   +  6 * cache[i-3]
                   - 72 * cache[i-4]
                   - 28 * cache[i-5]
                   + 74 * cache[i-6]
                   - 10 * cache[i-7]
                   -  4 * cache[i-8]
                   -  4 * cache[i-9];
    }
    last = n;

    return cache[n];
}
