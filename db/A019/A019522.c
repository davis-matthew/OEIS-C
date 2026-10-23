/*
 * A019522  Concatenate cubes.
 * https://oeis.org/A019522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Concatenate cubes: a(n) = concat_{k=1..n}k^3 */
#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[i-1] holds a(i) */
static size_t cache_len = 0;  /* number of terms currently cached */

/* raise 10 to a small exponent (<= 15 for these cubes) */
static seq_t pow10_small(int e)
{
    seq_t r = 1;
    for (int i = 0; i < e; ++i) r *= 10;
    return r;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0; /* offset starts at 1 */

    /* if we don't have enough terms cached, extend the cache */
    if ((size_t)n > cache_len) {
        /* allocate or grow the cache to at least n elements */
        seq_t *c = (seq_t *)realloc(cache,
                                    (size_t)n * sizeof(seq_t));
        if (!c) return 0;          /* allocation failed, return 0 silently */
        cache = c;

        /* compute missing terms, starting from cache_len+1 */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* k^3 */
            seq_t cube = (seq_t)i * i * i;

            /* number of decimal digits in cube */
            int digits = 0;
            seq_t tmp = cube;
            do {
                digits++;
                tmp /= 10;
            } while (tmp);

            seq_t factor = pow10_small(digits);

            if (i == 1) {
                cache[0] = cube;
            } else {
                cache[i - 1] = cache[i - 2] * factor + cube;
            }
        }
        cache_len = (size_t)n;
    }

    return cache[n - 1];
}
