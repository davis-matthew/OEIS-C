/*
 * A238913  Number of self-inverse permutations p on [n] where the maximal
 *          displacement of an element equals 2.
 * https://oeis.org/A238913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+a(n-2)-a(n-3)-a(n-4)-2*a(n-5)-a(n-6) with
 *            a(0)=0,a(1)=0,a(2)=0,a(3)=1,a(4)=3,a(5)=7
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 68, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n): number of self‑inverse permutations on [n] with maximal
   displacement exactly 2.  The sequence satisfies the recurrence derived
   from its rational generating function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;

    size_t target = (size_t)n;
    if (target >= cache_capacity) {
        /* enlarge storage */
        size_t new_cap = target + 1;
        if (new_cap < 6) new_cap = 6;        /* to hold the initial constants */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        if (!cache) return 0; /* allocation failure – unlikely in tests */

        if (cache_capacity == 0) {             /* first time: set base values */
            cache[0] = 0;
            cache[1] = 0;
            cache[2] = 0;
            cache[3] = 1;
            cache[4] = 3;
            cache[5] = 7;
        }

        /* fill in missing values using the recurrence */
        size_t start = cache_capacity;
        if (start < 6) start = 6;
        for (size_t i = start; i <= target; ++i) {
            /* compute 2*a(n-1) + a(n-2) */
            seq_t sum1 = cache[i-1];
            sum1 += cache[i-1];
            sum1 += cache[i-2];

            /* compute a(n-3) + a(n-4) + 2*a(n-5) + a(n-6) */
            seq_t sum2 = cache[i-3];
            sum2 += cache[i-4];
            sum2 += cache[i-5];
            sum2 += cache[i-5];          /* 2*a(n-5) */
            sum2 += cache[i-6];

            /* a(n) = sum1 - sum2;  sum1 >= sum2 */
            cache[i] = sum1 - sum2;
        }
        cache_capacity = new_cap;
    }
    return cache[n];
}
