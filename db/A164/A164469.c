/*
 * A164469  Number of binary strings of length n with no substrings equal to
 *          0001 0101 or 0110.
 * https://oeis.org/A164469
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-5) - a(n-6)
 *            with initial values a(4)=13, a(5)=20, a(6)=32, a(7)=51, a(8)=81,
 *            a(9)=128
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 47, u64 n <= 96, u128 n <= 194,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* realloc, abort */
#include <stddef.h>  /* size_t */

static const seq_t init_vals[6] = {13, 20, 32, 51, 81, 128};
static seq_t *cache = NULL;          /* cached sequence values      */
static size_t cache_size = 0;        /* number of cached entries     */
static const idx_t offset = 4;       /* first index with defined term */

seq_t a(idx_t n)
{
    /* For n < offset the sequence is undefined – return 0. */
    if (n < offset)
        return (seq_t)0;

    /* Index in the cache array (0 corresponds to a(4)). */
    size_t idx = (size_t)(n - offset);

    /* Grow the cache if needed. */
    if (idx >= cache_size) {
        size_t new_size = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            abort();                       /* out of memory */
        cache = tmp;

        /* Compute any new entries. */
        for (size_t i = cache_size; i < new_size; ++i) {
            if (i < 6) {
                cache[i] = init_vals[i];
            } else {
                /* Recurrence for n >= 10. */
                seq_t t = cache[i-1];
                t *= (seq_t)2;               /* 2*a(n-1) */
                t += cache[i-3];             /* + a(n-3) */
                t += cache[i-5];             /* + a(n-5) */
                seq_t sub = cache[i-2] + cache[i-4] + cache[i-6];
                t -= sub;                    /* subtract remaining terms */
                cache[i] = t;
            }
        }
        cache_size = new_size;
    }

    return cache[idx];
}
