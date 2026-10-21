/*
 * A219136  Unchanging value maps: number of n X 2 binary arrays indicating
 *          the locations of corresponding elements unequal to no horizontal,
 *          diagonal or antidiagonal neighbor in a random 0..2 n X 2 array.
 * https://oeis.org/A219136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 3*a(n-3) + 4*a(n-4) - 2*a(n-5) -
 *            4*a(n-6) Initial values (for n = 1 … 6): 2, 6, 18, 51, 142, 405
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* grow cache to at least needed elements */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **arr) {
    if (*cap < needed) {
        size_t new_cap = (*cap > 0) ? (*cap << 1) : 1;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);              /* allocation failure */
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;            /* undefined for n < 1 */

    /* initial values for n = 1 … 6 */
    static const seq_t init[6] = {2, 6, 18, 51, 142, 405};

    if (n <= 6) return init[n - 1];

    static seq_t *cache = NULL;
    static size_t cache_len = 0;    /* number of terms currently stored */
    static size_t cache_cap = 0;    /* allocated capacity */

    /* ensure cache has space for n terms */
    ensure_capacity((size_t)n, &cache_cap, &cache);

    /* initialise first six terms if not yet done */
    if (cache_len == 0) {
        for (size_t i = 0; i < 6; ++i) cache[i] = init[i];
        cache_len = 6;
    }

    /* compute any missing terms up to n */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        seq_t a6 = (i >= 6) ? cache[i - 6] : init[5 - (6 - i)]; /* not used for i<6 */

        seq_t positive = 2 * a1 + a2 + 3 * a3 + 4 * a4;
        seq_t negative = 2 * a5 + 4 * a6;
        cache[i] = positive - negative;
    }
    cache_len = (size_t)n;

    return cache[(size_t)n - 1];
}
