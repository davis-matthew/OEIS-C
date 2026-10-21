/*
 * A221484  Number of 2 X n arrays of occupancy after each element moves to
 *          some horizontal or antidiagonal neighbor, without consecutive
 *          moves in the same direction.
 * https://oeis.org/A221484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 17*a(n-2) + 22*a(n-3) + a(n-4) - 14*a(n-5) -
 *            2*a(n-6) + a(n-7) (for n ≥ 8) Initial values: a(1)=0, a(2)=4,
 *            a(3)=8, a(4)=73, a(5)=292, a(6)=1713, a(7)=8235
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Static cache that keeps previously computed terms. */
    static seq_t *cache     = NULL;   /* array of length cached_cap */
    static idx_t cached_cap = 0;      /* allocated slots   (≥ n+1) */
    static idx_t cached_max = 0;      /* highest index that has a valid value */

    /* Make sure the cache is large enough. */
    if (cached_cap <= n) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!tmp)                  /* allocation failure – abort gracefully */
            return (seq_t)0;
        cache     = tmp;
        cached_cap = n + 1;
    }

    /* Constant initial values for indices 1 … 7.
       The zero at index 0 is unused.  Use a uint64_t array – it can be cast
       to seq_t regardless of the concrete seq_t type. */
    static const uint64_t init_vals[8] = { 0, 0, 4, 8, 73, 292, 1713, 8235 };

    /* (Re)initialize the first seven entries (they are constants). */
    for (idx_t i = 1; i <= 7 && i <= n; ++i)
        cache[i] = (seq_t)init_vals[i];

    /* If we need to compute terms beyond what we have already, do it. */
    if (n > cached_max) {
        /* The recurrence is only valid from n = 8 onward. */
        idx_t start = (cached_max < 8 ? 8 : cached_max + 1);

        for (idx_t i = start; i <= n; ++i) {
            /* Positive part of the recurrence */
            seq_t pos = cache[i - 1];
            pos += (seq_t)17 * cache[i - 2];
            pos += (seq_t)22 * cache[i - 3];
            pos += cache[i - 4];
            pos += cache[i - 7];

            /* Negative part of the recurrence */
            seq_t neg = (seq_t)14 * cache[i - 5];
            neg += (seq_t)2 * cache[i - 6];

            cache[i] = pos - neg;
        }
        /* Record the new maximum index that has a valid value. */
        cached_max = n;
    }

    return cache[n];
}
