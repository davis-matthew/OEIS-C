/*
 * A222022  Number of binary arrays indicating the locations of trailing edge
 *          maxima of a random length-n 0..3 array extended with zeros and
 *          convolved with 1,3,3,1.
 * https://oeis.org/A222022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            a(n-1)+2*a(n-3)-a(n-4)+a(n-8)-a(n-9)+a(n-11)-a(n-12)-2*a(n-14)+a(n-15);
 *            Initial values: a(1)=2, a(2)=3, a(3)=4, a(4)=7, a(5)=11,
 *            a(6)=16, a(7)=26, a(8)=41, a(9)=62, a(10)=99, a(11)=156,
 *            a(12)=242, a(13)=382, a(14)=600, a(15)=938
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 49, u64 n <= 98, u128 n <= 196,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Pre‑computed first 15 values (index 0 unused). */
static const seq_t init[16] = {
    0ULL, /* 0th entry unused, keeps indices 1‑based */
    2ULL, 3ULL, 4ULL, 7ULL, 11ULL, 16ULL, 26ULL,
    41ULL, 62ULL, 99ULL, 156ULL, 242ULL, 382ULL,
    600ULL, 938ULL
};

/* Cached values of the sequence. */
static seq_t *cache      = NULL;   /* array of a(i), 1‑based */
static idx_t  cache_size = 0;      /* number of elements allocated (size ≥ 16) */
static idx_t  last_computed = 0;   /* largest n for which a(n) is stored */

/* Return a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for n<1, but safe return */

    /* For n < 16 we can answer immediately from the table. */
    if (n < 16) return init[n];

    /* Allocate cache on first request for n ≥ 16. */
    if (!cache) {
        /* Allocate enough for the first request plus some room for future calls. */
        cache_size = n + 1;
        if (cache_size < 16) cache_size = 16;
        cache = (seq_t*)calloc(cache_size, sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */
        /* Load the initial known values */
        for (idx_t i = 1; i < 16; ++i) cache[i] = init[i];
        last_computed = 15;
    }
    /* Enlarge cache if necessary for a current request. */
    else if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        /* Zero‑initialize the newly added part. */
        memset(tmp + cache_size, 0, (new_size - cache_size) * sizeof(seq_t));
        cache = tmp;
        cache_size = new_size;
    }

    /* If already computed, just return. */
    if (n <= last_computed) return cache[n];

    /* Compute missing values using the empirical recurrence. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val = cache[i - 1];
        val += 2 * cache[i - 3];
        val -= cache[i - 4];
        val += cache[i - 8];
        val -= cache[i - 9];
        val += cache[i - 11];
        val -= cache[i - 12];
        val -= 2 * cache[i - 14];
        val += cache[i - 15];
        cache[i] = val;
    }
    last_computed = n;
    return cache[n];
}
