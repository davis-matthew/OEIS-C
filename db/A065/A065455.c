/*
 * A065455  Number of (binary) bit strings of length n in which no even block
 *          of 0's is followed by an odd block of 1's.
 * https://oeis.org/A065455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) + a(n-3) (a(0)=1, a(1)=2, a(2)=4)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 70, u128 n <= 140,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for memoizing computed terms */
static seq_t *cache = NULL;
static idx_t cached_upto = 0;

/* Return the nth term of A065455 */
seq_t a(idx_t n)
{
    /* Small n handled directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;

    /* If cache is not big enough, extend it */
    if (n > cached_upto) {
        idx_t new_size = n + 1;
        seq_t *new_cache;

        if (!cache) {
            new_cache = (seq_t*)malloc(sizeof(seq_t) * new_size);
            if (!new_cache) return (seq_t)0; /* allocation failed (unlikely) */

            new_cache[0] = (seq_t)1;
            new_cache[1] = (seq_t)2;
            new_cache[2] = (seq_t)4;

            for (idx_t i = 3; i <= n; ++i)
                new_cache[i] = new_cache[i-2] * (seq_t)3 + new_cache[i-3];

            cache = new_cache;
            cached_upto = n;
        } else {
            new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
            if (!new_cache) return (seq_t)0; /* allocation failed */

            /* Compute missing terms */
            for (idx_t i = cached_upto + 1; i <= n; ++i)
                new_cache[i] = new_cache[i-2] * (seq_t)3 + new_cache[i-3];

            cache = new_cache;
            cached_upto = n;
        }
    }
    return cache[n];
}
