/*
 * A143642  Numerators of principal and intermediate convergents to 3^(1/2).
 * https://oeis.org/A143642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 50, u64 n <= 101,
 *            u128 n <= 202, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-3) - a(n-6) for n > 6,
   initial values: a(1)=1, a(2)=2, a(3)=3, a(4)=5, a(5)=7, a(6)=12 */
#include <stdlib.h>

static seq_t *cache = NULL;  /* memoization array, index 0 unused */
static idx_t cache_size = 0; /* highest index that has been computed */

/* Numerators of principal and intermediate convergents to sqrt(3) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not part of the OEIS offset */

    /* Lazily allocate and initialise cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)7 * sizeof(seq_t)); /* indices 0..6 */
        if (cache == NULL) return 0; /* allocation failure */
        cache[0] = 0;     /* unused */
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 3;
        cache[4] = 5;
        cache[5] = 7;
        cache[6] = 12;
        cache_size = 6;
    }

    /* Grow cache if n exceeds the current maximum */
    if (n > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)(n + 1) * sizeof(seq_t));
        if (tmp == NULL) return 0; /* allocation failure */
        cache = tmp;
        /* Compute the new terms using the recurrence */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            cache[i] = 4 * cache[i - 3] - cache[i - 6];
        }
        cache_size = n;
    }

    return cache[n];
}
