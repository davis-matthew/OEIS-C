/*
 * A041877  Denominators of continued fraction convergents to sqrt(460).
 * https://oeis.org/A041877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 5071502*a(n-12) - a(n-24) for n>23; initial terms (n =
 *            0..23) are: 1, 2, 9, 29, 38, 105, 1088, 2281, 3369, 12388,
 *            52921, 118230, 5018581, 10155392, 45640149, 147075839,
 *            192715988, 532507815, 5517794138, 11568096091, 17085890229,
 *            62825766778, 268388957341, 599603681460
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;
    static const seq_t init_vals[24] = {
        1ULL, 2ULL, 9ULL, 29ULL, 38ULL, 105ULL, 1088ULL, 2281ULL, 3369ULL, 12388ULL,
        52921ULL, 118230ULL, 5018581ULL, 10155392ULL, 45640149ULL, 147075839ULL,
        192715988ULL, 532507815ULL, 5517794138ULL, 11568096091ULL,
        17085890229ULL, 62825766778ULL, 268388957341ULL, 599603681460ULL
    };
    const idx_t init_len = 24;
    const seq_t coef = (seq_t)5071502;

    /* Initialise cache with the first 24 terms once */
    if (cache == NULL) {
        cache = (seq_t *)malloc(init_len * sizeof(seq_t));
        if (!cache) abort();
        for (idx_t i = 0; i < init_len; ++i)
            cache[i] = init_vals[i];
        cached_size = init_len;
    }

    /* Fast return if already computed */
    if (n < cached_size)
        return cache[n];

    /* Extend cache to at least n+1 entries */
    idx_t new_size = n + 1;
    if (new_size < init_len)
        new_size = init_len;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* Compute missing terms using the recurrence */
    for (idx_t i = cached_size; i <= n; ++i) {
        /* i >= 24 here */
        cache[i] = coef * cache[i - 12] - cache[i - 24];
    }
    cached_size = new_size;
    return cache[n];
}
