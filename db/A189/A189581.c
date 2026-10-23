/*
 * A189581  Number of permutations of 1..n with displacements restricted to
 *          {-4,-3,0,1,2}.
 * https://oeis.org/A189581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 61, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(n) = a(n-1) + 2*a(n-3) + 3*a(n-4) + 5*a(n-5) + 5*a(n-6)
          - a(n-8) - 6*a(n-9) - a(n-10) - 3*a(n-11) - a(n-12)
          + a(n-13) + a(n-15)
   with initial values:
   a(1)=1, a(2)=1, a(3)=1, a(4)=4, a(5)=13, a(6)=26, a(7)=48,
   a(8)=95, a(9)=205, a(10)=456, a(11)=974, a(12)=2024,
   a(13)=4225, a(14)=8930, a(15)=18977. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cached values 0..cache_size */
    static idx_t cache_size = 0;    /* largest n cached so far */

    /* Predefined initial values for indices 1..15 */
    static const seq_t init[16] = {
        0ULL,            /* dummy for index 0 */
        1ULL, 1ULL, 1ULL, 4ULL, 13ULL, 26ULL,
        48ULL, 95ULL, 205ULL, 456ULL, 974ULL,
        2024ULL, 4225ULL, 8930ULL, 18977ULL
    };

    /* Return already computed value */
    if (n <= cache_size)
        return cache[n];

    /* Growing cache: keep the old part, allocate a larger array. */
    idx_t old_size = cache_size;
    seq_t *new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed – terminate the program. */
        exit(EXIT_FAILURE);
    }

    if (cache) {
        /* Copy existing values. */
        memcpy(new_cache, cache, (old_size + 1) * sizeof(seq_t));
        free(cache);
    } else {
        /* First call: set any existing element to 0 for safety. */
        new_cache[0] = 0;
    }

    cache = new_cache;
    cache_size = n;

    /* Fill in the initial values that are not yet present. */
    if (old_size < 15) {
        for (idx_t i = old_size + 1; i <= 15 && i <= n; ++i)
            cache[i] = init[i];
    }

    /* Compute new entries using the recurrence. */
    for (idx_t i = (old_size + 1 > 15 ? old_size + 1 : 16); i <= n; ++i) {
        seq_t val = cache[i - 1];
        val += (seq_t)2 * cache[i - 3];
        val += (seq_t)3 * cache[i - 4];
        val += (seq_t)5 * cache[i - 5];
        val += (seq_t)5 * cache[i - 6];
        val -= cache[i - 8];
        val -= (seq_t)6 * cache[i - 9];
        val -= cache[i - 10];
        val -= (seq_t)3 * cache[i - 11];
        val -= cache[i - 12];
        val += cache[i - 13];
        val += cache[i - 15];
        cache[i] = val;
    }

    return cache[n];
}
