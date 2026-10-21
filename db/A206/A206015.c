/*
 * A206015  Number of (n+1) X 3 0..3 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock unequal to the number of
 *          counterclockwise edge increases.
 * https://oeis.org/A206015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 23*a(n-2) - 94*a(n-3) + 216*a(n-4) -
 *            64*a(n-5) for n ≥ 6, with a(1)=680, a(2)=7688, a(3)=87448,
 *            a(4)=996840, a(5)=11364568. (The recurrence is derived from OEIS
 *            sequence A206015.)
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define C14  ((seq_t)14)
#define C23  ((seq_t)23)
#define C94  ((seq_t)94)
#define C216 ((seq_t)216)
#define C64  ((seq_t)64)

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;  /* a(1) stored at cache[0] */
    static size_t cache_cap = 0; /* number of terms stored */

    if (n == 0) return 0; /* offset 1; optional safety */

    /* Ensure array big enough */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        if (new_cap < 5) new_cap = 5;  /* reserve space for base */
        seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_arr) exit(1);

        /* copy old data if any */
        if (cache)
            memcpy(new_arr, cache, cache_cap * sizeof(seq_t));

        /* initialise base values on first allocation */
        if (cache_cap == 0) {
            new_arr[0] = 680;        /* a(1) */
            new_arr[1] = 7688;       /* a(2) */
            new_arr[2] = 87448;      /* a(3) */
            new_arr[3] = 996840;     /* a(4) */
            new_arr[4] = 11364568;   /* a(5) */
            cache_cap = 5;
        }

        /* compute further terms via recurrence */
        for (size_t i = cache_cap; i < (size_t)n; ++i) {
            new_arr[i] = C14 * new_arr[i-1]
                        - C23 * new_arr[i-2]
                        - C94 * new_arr[i-3]
                        + C216 * new_arr[i-4]
                        - C64 * new_arr[i-5];
        }

        free(cache);
        cache = new_arr;
        /* cache_cap may remain the same (previous size) or larger,
           but we just need at least n elements stored. */
        if (n > cache_cap) cache_cap = n;
    }

    return cache[(size_t)(n - 1)];
}
