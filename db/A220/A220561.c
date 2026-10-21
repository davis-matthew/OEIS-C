/*
 * A220561  Number of ways to reciprocally link elements of an nX7 array
 *          either to themselves or to exactly one horizontal or antidiagonal
 *          neighbor.
 * https://oeis.org/A220561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 85*a(n-1) – 1330*a(n-2) + 7350*a(n-3) – 15316*a(n-4) +
 *            12720*a(n-5) – 3168*a(n-6) (for n > 9) Initial values: a(1) =
 *            21, a(2) = 1544, a(3) = 101052, a(4) = 6743873, a(5) =
 *            449429440, a(6) = 29955561052, a(7) = 1996515036228, a(8) =
 *            133064181801048, a(9) = 8868465829053416
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cached values of the sequence, 1‑indexed (cache[0] = a(1)) */
static const seq_t init_vals[9] = {
    21,
    1544,
    101052,
    6743873,
    449429440,
    29955561052ULL,
    1996515036228ULL,
    133064181801048ULL,
    8868465829053416ULL
};

/* dynamic cache for computed terms */
static seq_t *cache = NULL;          /* array of computed terms */
static size_t cache_len = 0;         /* number of terms currently stored */
static size_t cache_cap = 0;         /* allocated capacity */

/* ensure that cache contains terms up to and including index n (1‑based) */
static void ensure_cache(size_t n)
{
    /* initialise cache if not yet done */
    if (cache == NULL) {
        cache_cap = n > 10 ? n : 10;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* copy initial terms */
        for (size_t i = 0; i < 9; ++i)
            cache[i] = init_vals[i];
        cache_len = n < 9 ? n : 9;
    } else if (n > cache_cap) {
        /* grow capacity to at least n */
        size_t new_cap = cache_cap;
        while (new_cap < n)
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing terms if needed */
    if (n > cache_len) {
        for (size_t i = cache_len + 1; i <= n; ++i) {
            size_t idx = i - 1; /* position in cache */
            cache[idx] = (seq_t)85   * cache[idx - 1]
                      - (seq_t)1330  * cache[idx - 2]
                      + (seq_t)7350  * cache[idx - 3]
                      - (seq_t)15316 * cache[idx - 4]
                      + (seq_t)12720 * cache[idx - 5]
                      - (seq_t)3168  * cache[idx - 6];
        }
        cache_len = n;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)                  /* behaviour for n < 1 is irrelevant */
        return (seq_t)0;
    size_t un = (size_t)n;      /* safe cast for indexing */

    if (un <= 9)
        return init_vals[un - 1];

    ensure_cache(un);
    return cache[un - 1];
}
