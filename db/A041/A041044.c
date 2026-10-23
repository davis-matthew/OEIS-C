/*
 * A041044  Numerators of continued fraction convergents to sqrt(28).
 * https://oeis.org/A041044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 254*a(n-4) - a(n-8); a(0)=5,a(1)=16,a(2)=37,a(3)=127,
 *            a(4)=1307,a(5)=4048,a(6)=9403,a(7)=32257
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* public function */
seq_t a(idx_t n)
{
    /*
     * Static memoization buffer.  The buffer is grown as needed to
     * hold all values up to the largest n requested so far.
     */
    static seq_t *cache = NULL;     /* array of computed terms          */
    static size_t cache_len = 0;    /* current allocated length         */

    /* constants for initialization */
    static const seq_t init_vals[8] = {
        5, 16, 37, 127, 1307, 4048, 9403, 32257
    };

    /* If we don't yet have enough space, grow the buffer */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        if (new_len < 8) new_len = 8;            /* at least 8 slots for init */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;                      /* allocation error */
        cache = tmp;

        /* Initialize the first eight terms on the very first allocation */
        if (cache_len == 0) {
            for (size_t i = 0; i < 8; ++i) cache[i] = init_vals[i];
        }

        /* Compute new terms up to n using the recurrence */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i < 8) continue;                  /* already set */
            cache[i] = 254 * cache[i - 4] - cache[i - 8];
        }
        cache_len = n + 1;
    }

    return cache[(size_t)n];
}
