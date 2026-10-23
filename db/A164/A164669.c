/*
 * A164669  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164669: Number of reduced words of length n in the
   Coxeter group on 33 generators with relations (S_i)^2 = (S_i S_j)^7 = I.
   The sequence satisfies the linear recurrence
       a(0) = 1, a(1) = 33, a(2) = 1056, a(3) = 33792,
       a(4) = 1081344, a(5) = 34603008, a(6) = 1107296256,
       a(7) = 35433479664;
   for n ≥ 8:
       a(n) = 31*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)) - 496*a(n-7).
*/

#include <stdlib.h>
#include <stdbool.h>

/* Initial values for indices 0..7.  Cast to seq_t for any type. */
static const seq_t init[8] = {
    (seq_t)1ULL,
    (seq_t)33ULL,
    (seq_t)1056ULL,
    (seq_t)33792ULL,
    (seq_t)1081344ULL,
    (seq_t)34603008ULL,
    (seq_t)1107296256ULL,
    (seq_t)35433479664ULL
};

seq_t a(idx_t n)
{
    /* Static cache to store computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;    /* allocated capacity */
    static idx_t cached_until = 0;   /* largest index computed (>=7 after init) */
    static bool initialized = false;

    /* One‑time initialization of the cache with the first 8 values */
    if (!initialized) {
        cache_size   = 8;
        cache        = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) exit(1);     /* memory exhaustion – abort */
        for (size_t i = 0; i < 8; ++i) cache[i] = init[i];
        cached_until = 7;
        initialized  = true;
    }

    /* If the requested index is already cached, return it directly */
    if (n <= cached_until) return cache[n];

    /* Allocate more space if needed */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);   /* memory exhaustion – abort */
        cache       = tmp;
        cache_size  = new_size;
    }

    /* Compute values sequentially up to n, storing them in the cache */
    for (idx_t i = cached_until + 1; i <= n; ++i) {
        /* For i <= 7 the values already exist; this loop starts at >=8 */
        seq_t sum = cache[i-1];
        sum += cache[i-2];
        sum += cache[i-3];
        sum += cache[i-4];
        sum += cache[i-5];
        sum += cache[i-6];
        sum = sum * (seq_t)31U;
        seq_t val = sum - (seq_t)496U * cache[i-7];
        cache[i] = val;
    }
    cached_until = n;
    return cache[n];
}
