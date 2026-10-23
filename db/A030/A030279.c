/*
 * A030279  COMPOSE squares with squares.
 * https://oeis.org/A030279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 12*a(n-1)-54*a(n-2)+115*a(n-3)-132*a(n-4)+108*a(n-5)
 *            -59*a(n-6)+24*a(n-7)-6*a(n-8)+a(n-9) Initial values: a(1)=1,
 *            a(2)=8, a(3)=50, a(4)=276, a(5)=1397, a(6)=6672, a(7)=30565,
 *            a(8)=135668, a(9)=587426
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init_values[9] = {
    (seq_t)1,
    (seq_t)8,
    (seq_t)50,
    (seq_t)276,
    (seq_t)1397,
    (seq_t)6672,
    (seq_t)30565,
    (seq_t)135668,
    (seq_t)587426
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset 1: values are defined for n >= 1 */

    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t computed = 0; /* number of terms currently stored */

    /* First use: allocate cache and store the initial terms */
    if (cache == NULL) {
        capacity = 16;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure guard */

        for (size_t i = 0; i < 9; ++i) {
            cache[i] = init_values[i];
        }
        computed = 9;
    }

    /* Return a previously computed term if requested */
    if ((size_t)n <= computed) {
        return cache[(size_t)n - 1];
    }

    /* Grow cache if necessary */
    if ((size_t)n > capacity) {
        size_t new_cap = capacity ? capacity * 2 : 16;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0; /* allocation failure guard */
        cache = new_cache;
        capacity = new_cap;
    }

    /* Compute terms from the first missing term up to n using the recurrence */
    for (size_t i = computed + 1; i <= (size_t)n; ++i) {
        seq_t val = (seq_t)12 * cache[i - 2];      /* a(i-1) */
        val -= (seq_t)54 * cache[i - 3];           /* a(i-2) */
        val += (seq_t)115 * cache[i - 4];          /* a(i-3) */
        val -= (seq_t)132 * cache[i - 5];          /* a(i-4) */
        val += (seq_t)108 * cache[i - 6];          /* a(i-5) */
        val -= (seq_t)59 * cache[i - 7];           /* a(i-6) */
        val += (seq_t)24 * cache[i - 8];           /* a(i-7) */
        val -= (seq_t)6 * cache[i - 9];            /* a(i-8) */
        val += cache[i - 10];                     /* a(i-9) */
        cache[i - 1] = val;
    }

    computed = (size_t)n;
    return cache[(size_t)n - 1];
}
