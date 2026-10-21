/*
 * A221458  Number of 0..7 arrays of length n with each element unequal to at
 *          least one neighbor, with new values introduced in 0..7 order.
 * https://oeis.org/A221458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 15, u64 n <= 26, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence): a(n) = 22*a(n-1) -168*a(n-2) +440*a(n-3) +421*a(n-4)
   -2898*a(n-5) -924*a(n-6) +7944*a(n-7) +5931*a(n-8) -6610*a(n-9)
   -10562*a(n-10) -5040*a(n-11) -840*a(n-12)  (n >= 13)
   Initial values:
   a(1)=0, a(2)=1, a(3)=2, a(4)=7, a(5)=25, a(6)=102, a(7)=456,
   a(8)=2219, a(9)=11639, a(10)=65320, a(11)=389533, a(12)=2451393 */

#include <stdlib.h>

static seq_t compute_term(const seq_t *arr, idx_t i)
{
    seq_t pos = (seq_t)22 * arr[i-1];
    pos += (seq_t)440 * arr[i-3];
    pos += (seq_t)421 * arr[i-4];
    pos += (seq_t)7944 * arr[i-7];
    pos += (seq_t)5931 * arr[i-8];

    seq_t neg = (seq_t)168 * arr[i-2];
    neg += (seq_t)2898 * arr[i-5];
    neg += (seq_t)924 * arr[i-6];
    neg += (seq_t)6610 * arr[i-9];
    neg += (seq_t)10562 * arr[i-10];
    neg += (seq_t)5040 * arr[i-11];
    neg += (seq_t)840 * arr[i-12];

    return pos - neg;
}

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n <= cache_len && cache != NULL)
        return cache[n];

    /* Initialise cache with first 12 values if needed */
    if (cache == NULL) {
        cache = (seq_t*)malloc((12 + 1) * sizeof(seq_t));
        if (!cache) exit(1);          /* Allocation failed – terminate */
        cache[0] = 0;                  /* unused */
        cache[1] = 0;
        cache[2] = 1;
        cache[3] = 2;
        cache[4] = 7;
        cache[5] = 25;
        cache[6] = 102;
        cache[7] = 456;
        cache[8] = 2219;
        cache[9] = 11639;
        cache[10] = 65320;
        cache[11] = 389533;
        cache[12] = 2451393;
        cache_len = 12;
    }

    /* Extend cache to accommodate n */
    if (n > cache_len) {
        seq_t *new_cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) {
            free(cache);
            exit(1);
        }
        cache = new_cache;
        for (idx_t i = cache_len + 1; i <= n; ++i)
            cache[i] = compute_term(cache, i);
        cache_len = n;
    }

    return cache[n];
}
