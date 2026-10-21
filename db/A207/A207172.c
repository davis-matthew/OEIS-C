/*
 * A207172  Number of 4 X n 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 13*a(n-3) + 3*a(n-4) + 3*a(n-5) * - 27*a(n-6) -
 *            18*a(n-7) + 27*a(n-9) for n > 11 * Initial values (offset 1):
 *            a(1)=8, a(2)=64, a(3)=168, a(4)=603, * a(5)=1612, a(6)=3952,
 *            a(7)=11452, a(8)=32021, a(9)=84300, * a(10)=231616,
 *            a(11)=641775, a(12)=1736910
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 41, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* 1‑based cache, cache[0] unused  */
static idx_t  cache_size = 0;    /* highest index computed  */
static size_t cache_cap  = 0;    /* allocated capacity (bytes of array elements) */

/* Compute a(n) for any n >= 1.  Results are cached for future calls. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* below offset – behaviour not required */

    /* initialise cache with the first 12 terms and a small capacity */
    if (cache == NULL) {
        cache_cap  = 32;                      /* initial capacity */
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                  /* allocation failure – unlikely */

        cache[1]  = (seq_t)8;
        cache[2]  = (seq_t)64;
        cache[3]  = (seq_t)168;
        cache[4]  = (seq_t)603;
        cache[5]  = (seq_t)1612;
        cache[6]  = (seq_t)3952;
        cache[7]  = (seq_t)11452;
        cache[8]  = (seq_t)32021;
        cache[9]  = (seq_t)84300;
        cache[10] = (seq_t)231616;
        cache[11] = (seq_t)641775;
        cache[12] = (seq_t)1736910;
        cache_size = 12;
    }

    /* enlarge storage if required */
    if (n > (idx_t)cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;      /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                                /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* return already computed value */
    if (n <= cache_size) return cache[n];

    /* compute values up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* recurrence for i > 12 (i > 11 by definition) */
        seq_t sum1 = cache[i-1];
        sum1 += (seq_t)13 * cache[i-3];
        sum1 += (seq_t)3  * cache[i-4];
        sum1 += (seq_t)3  * cache[i-5];
        sum1 += (seq_t)27 * cache[i-9];

        seq_t sum2 = (seq_t)27 * cache[i-6];
        sum2 += (seq_t)18 * cache[i-7];

        cache[i] = sum1 - sum2;
    }
    cache_size = n;
    return cache[n];
}
