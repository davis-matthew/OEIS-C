/*
 * A218998  Unmatched value maps: number of nX4 binary arrays indicating the
 *          locations of corresponding elements not equal to any horizontal,
 *          diagonal or antidiagonal neighbor in a random 0..2 nX4 array.
 * https://oeis.org/A218998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) + 42*a(n-2) + 282*a(n-3) + 281*a(n-4) +
 *            148*a(n-5) - 320*a(n-6) - 320*a(n-7) - 256*a(n-8) Initial
 *            values: a(1) = 7, a(2) = 163, a(3) = 2265, a(4) = 31857, a(5) =
 *            461681, a(6) = 6639893, a(7) = 95431043, a(8) = 1372612359
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init_values[8] = {
    7ULL,           /* a(1) */
    163ULL,         /* a(2) */
    2265ULL,        /* a(3) */
    31857ULL,       /* a(4) */
    461681ULL,      /* a(5) */
    6639893ULL,     /* a(6) */
    95431043ULL,    /* a(7) */
    1372612359ULL   /* a(8) */
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined; return 0 */

    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t capacity = 0;           /* allocated slots */
    static size_t computed = 0;           /* how many terms are actually stored */

    /* Initialize cache with first 8 terms */
    if (!cache) {
        capacity = 8;
#ifdef __cplusplus
        cache = new seq_t[capacity];
#else
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
#endif
        if (!cache) abort();              /* out of memory */
        for (size_t i = 0; i < 8; ++i) cache[i] = init_values[i];
        computed = 8;
    }

    /* Grow cache if necessary */
    if (capacity < (size_t)n) {
#ifdef __cplusplus
        seq_t *tmp = new seq_t[n];
        for (size_t i = 0; i < capacity; ++i) tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
#else
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) abort();                 /* out of memory */
        cache = tmp;
#endif
        capacity = n;
    }

    /* Compute missing terms using the recurrence */
    size_t start = computed;
    if (start < 8) start = 8;              /* recurrence needs 8 previous terms */
    for (idx_t i = start; i < n; ++i) {    /* i is zero‑based index (a(i+1)) */
        seq_t term = ((seq_t)10) * cache[i-1] +
                     ((seq_t)42) * cache[i-2] +
                     ((seq_t)282) * cache[i-3] +
                     ((seq_t)281) * cache[i-4] +
                     ((seq_t)148) * cache[i-5] -
                     ((seq_t)320) * cache[i-6] -
                     ((seq_t)320) * cache[i-7] -
                     ((seq_t)256) * cache[i-8];
        cache[i] = term;
    }
    if (computed < (size_t)n) computed = n;

    return cache[n-1];
}
