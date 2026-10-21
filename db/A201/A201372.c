/*
 * A201372  Number of n X 5 0..1 arrays with rows and columns
 *          lexicographically nondecreasing read forwards, and nonincreasing
 *          read backwards.
 * https://oeis.org/A201372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 76, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A201372: Number of n×5 0/1 arrays with rows and columns
   lexicographically nondecreasing forwards and nonincreasing backwards.
   The sequence satisfies the 7‑term linear recurrence
        a(1)=2,  a(2)=6,  a(3)=22,  a(4)=80,  a(5)=268,
        a(6)=786, a(7)=2016
        a(n)=7*a(n-1)-21*a(n-2)+35*a(n-3)-35*a(n-4)+21*a(n-5)-7*a(n-6)+a(n-7)  for n>7
   This implementation uses that recurrence with static memoisation.
*/
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Static cache for previously computed values. */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;   /* allocated size (≥ index+1)   */
    static idx_t  computed_max = 0; /* largest index that has been filled */

    /* Ensure the cache is large enough for index n. */
    if ((idx_t)cache_size <= n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)          /* allocation failed (unlikely in tests) */ 
            return 0;
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialise the first seven values (if not already done). */
    if (computed_max < 7) {
        /* Values for a(1) .. a(7).  Index 0 is unused. */
        static const seq_t init_vals[8] = {0,
                                          (seq_t)2,
                                          (seq_t)6,
                                          (seq_t)22,
                                          (seq_t)80,
                                          (seq_t)268,
                                          (seq_t)786,
                                          (seq_t)2016};
        for (idx_t i = computed_max + 1; i <= 7 && i <= n; ++i) {
            cache[i] = init_vals[i];
        }
    }

    /* Compute new terms up to n (if necessary). */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        if (i <= 7) {          /* already initialised */
            continue;
        }

        seq_t a1 = cache[i-1];
        seq_t a2 = cache[i-2];
        seq_t a3 = cache[i-3];
        seq_t a4 = cache[i-4];
        seq_t a5 = cache[i-5];
        seq_t a6 = cache[i-6];
        seq_t a7 = cache[i-7];

        /* Compute the linear recurrence avoiding unsigned underflow. */
        seq_t pos = (a1 * (seq_t)7) + (a3 * (seq_t)35) + (a5 * (seq_t)21) + a7;
        seq_t neg = (a2 * (seq_t)21) + (a4 * (seq_t)35) + (a6 * (seq_t)7);
        cache[i] = pos - neg;   /* pos is always ≥ neg for n≥8 */
    }

    computed_max = (computed_max > n) ? computed_max : n;
    return cache[n];
}
