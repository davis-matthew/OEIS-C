/*
 * A042528  Numerators of continued fraction convergents to sqrt(793).
 * https://oeis.org/A042528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 8786*a(n-4) - a(n-8) Initial values: a(0)=28, a(1)=169,
 *            a(2)=704, a(3)=4393, a(4)=246712, a(5)=1484665, a(6)=6185372,
 *            a(7)=38596897
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */
#include <stdlib.h> /* for malloc, realloc */

static const seq_t init_vals[8] = {
    (seq_t)28,
    (seq_t)169,
    (seq_t)704,
    (seq_t)4393,
    (seq_t)246712,
    (seq_t)1484665,
    (seq_t)6185372,
    (seq_t)38596897
};

static seq_t *cache = NULL;
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Initialize cache with the first 8 terms */
    if (cache == NULL) {
        cache_len = (size_t)n + 1;
        if (cache_len < 8) cache_len = 8;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        for (size_t i = 0; i < 8; ++i) cache[i] = init_vals[i];
    }

    /* Extend cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)8786 * cache[i - 4] - cache[i - 8];
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
