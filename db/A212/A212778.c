/*
 * A212778  Half the number of 0..4 arrays of length n+2 with second
 *          differences nonzero.
 * https://oeis.org/A212778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 2*a(n-1) + 7*a(n-2) + 14*a(n-3) + 19*a(n-4) +
          18*a(n-5) + 5*a(n-6) - 5*a(n-7) - 2*a(n-8)   (n ≥ 9)
   Initial values (a(1) … a(8)):
   56, 252, 1134, 5104, 22972, 103391, 465336, 2094360
*/
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* Empty sequence or offset below 1 is undefined; return 0 */
    if (n == 0) return 0;

    static seq_t *cache = NULL;
    static size_t cached_size = 0;      /* number of allocated slots (cache size) */

    /* Ensure the cache holds indices up to n */
    if ((size_t)n >= cached_size)
    {
        /* Compute required size (at least 9 to hold initial values) */
        size_t new_size = (size_t)n + 1;
        if (new_size < 9) new_size = 9;

        seq_t *tmp = (cache == NULL) ? (seq_t*)malloc(new_size * sizeof(seq_t))
                                      : (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            abort();               /* memory allocation failure */
        cache = tmp;

        /* Initialize cache if first time */
        if (cached_size == 0)
        {
            cache[0] = 0;              /* unused, maintain 1‑based indexing */
            cache[1] = 56;
            cache[2] = 252;
            cache[3] = 1134;
            cache[4] = 5104;
            cache[5] = 22972;
            cache[6] = 103391;
            cache[7] = 465336;
            cache[8] = 2094360;
            cached_size = 9;           /* indices 0..8 filled */
        }

        /* Compute any newly required terms using the recurrence */
        for (idx_t i = cached_size; i <= n; ++i)
        {
            seq_t pos = 2*cache[i-1] + 7*cache[i-2] + 14*cache[i-3] +
                        19*cache[i-4] + 18*cache[i-5] + 5*cache[i-6];
            seq_t neg = 5*cache[i-7] + 2*cache[i-8];
            cache[i] = pos - neg;      /* pos ≥ neg by sequence definition */
        }

        cached_size = new_size;
    }

    return cache[n];
}
