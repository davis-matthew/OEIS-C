/*
 * A073553  Number of Fibonacci numbers F(k), k <= 10^n, which end in 5.
 * https://oeis.org/A073553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (2*10^n + 10)/15, equivalently * a(1) = 2, a(2) = 14,
 *            a(n) = 11*a(n-1) - 10*a(n-2) for n > 2 * (OEIS A073553)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* sequence cache: cache[1..max_n] filled */
static idx_t computed_upto = 0; /* largest n that has been computed */

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n < 1 */

    /* Initialise cache on first call */
    if (cache == NULL) {
        /* Allocate space for indices 0..2 (ignore 0) */
        cache = (seq_t*)malloc(3 * sizeof(seq_t));
        cache[1] = (seq_t)2;   /* a(1) */
        cache[2] = (seq_t)14;  /* a(2) */
        computed_upto = 2;
    }

    /* Expand cache if necessary and compute missing terms */
    if (n > computed_upto) {
        idx_t new_size = n + 1;  /* +1 to have entry for index n */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return cache[n]; /* unlikely, but safety */
        cache = tmp;

        for (idx_t i = computed_upto + 1; i <= n; ++i) {
            /* a(i) = 11*a(i-1) - 10*a(i-2) */
            seq_t val = 11 * cache[i-1];
            val -= 10 * cache[i-2];
            cache[i] = val;
        }
        computed_upto = n;
    }

    return cache[n];
}
