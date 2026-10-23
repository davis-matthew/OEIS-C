/*
 * A166766  Number of n X 4 1..2 arrays containing at least one of each value,
 *          and all equal values connected.
 * https://oeis.org/A166766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 34, u128 n <= 71,
 *            big any n
 * Helpers:   free_seq, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 10*a(n-1) - 37*a(n-2) + 59*a(n-3) - 27*a(n-4)
 *                 - 25*a(n-5) + 21*a(n-6) + 5*a(n-7) - 4*a(n-8) - a(n-9)  for n > 9
 * Initial values: a(1)=6, a(2)=56, a(3)=292, a(4)=1254,
 *                 a(5)=4942, a(6)=18584, a(7)=67756, a(8)=241542, a(9)=846502
 */

#include <stddef.h>                  /* size_t */
#include "../utils/seq_alloc_array.h" /* seq_alloc_array() */
#include "../utils/free_seq.h"       /* free_seq() */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* out of range, undefined in OEIS */

    static seq_t *cache      = NULL;    /* computed values, index 0..len-1   */
    static size_t  capacity = 0;        /* allocated capacity (>= len)      */
    static idx_t   computed = 0;        /* number of values computed (len)  */

    /* Initialise the first 10 slots (indices 0..9) with base values */
    if (!cache) {
        capacity = 10;
        cache    = seq_alloc_array(capacity);
        computed = capacity;           /* indices 0..9 are valid            */
        cache[0] = 0;
        cache[1] = 6;
        cache[2] = 56;
        cache[3] = 292;
        cache[4] = 1254;
        cache[5] = 4942;
        cache[6] = 18584;
        cache[7] = 67756;
        cache[8] = 241542;
        cache[9] = 846502;
    }

    /* Grow the array if needed */
    if ((size_t)(n + 1) > capacity) {
        size_t new_cap = capacity ? capacity * 2 : 10;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;

        seq_t *new_cache = seq_alloc_array(new_cap);
        for (size_t i = 0; i < capacity; ++i)
            new_cache[i] = cache[i];

        free_seq(cache);
        cache    = new_cache;
        capacity = new_cap;
    }

    /* Compute new values up to n */
    for (idx_t i = computed; i <= n; ++i) {
        if (i <= 9)
            continue;                  /* already set */

        seq_t val = 10 * cache[i - 1]
                  - 37 * cache[i - 2]
                  + 59 * cache[i - 3]
                  - 27 * cache[i - 4]
                  - 25 * cache[i - 5]
                  + 21 * cache[i - 6]
                  + 5  * cache[i - 7]
                  - 4  * cache[i - 8]
                  -   cache[i - 9];
        cache[i] = val;
    }

    if (computed <= n)
        computed = n + 1;

    return cache[n];
}
