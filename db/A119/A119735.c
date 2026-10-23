/*
 * A119735  Numbers n such that every digit occurs at least once in n^3.
 * https://oeis.org/A119735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that every decimal digit 0‑9 occurs in n³ */

/* Standard C/C++ includes */
#include <stdbool.h>
#include <stddef.h>
#ifdef __cplusplus
    /* In C++ use new[] / delete[] to construct the array if seq_t is an
       arbitrary‑precision class.  In C use malloc/free.  */
    #define ALLOC_SEQ_T(arr,cap) ((arr)=new seq_t[cap])
    #define FREE_SEQ_T(arr)      (delete[] (arr))
#else
    #include <stdlib.h>
    #define ALLOC_SEQ_T(arr,cap) ((arr)= (seq_t*)malloc((cap)*sizeof(seq_t)))
    #define FREE_SEQ_T(arr)      (free(arr))
#endif

/* Internal cache state */
static seq_t *cache = NULL;      /* array of found terms */
static size_t cache_size = 0;    /* number of terms stored */
static size_t cache_cap  = 0;    /* current capacity of cache array */
static idx_t  last_candidate = 0;/* last integer examined */

/* Grow the cache array to hold at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    if (cache_cap >= new_cap)
        return;
    /* Allocate a new buffer */
    seq_t *tmp = NULL;
    ALLOC_SEQ_T(tmp, new_cap);
    /* Copy existing terms */
    for (size_t i = 0; i < cache_size; ++i)
        tmp[i] = cache[i];
    /* Free old buffer and update state */
    if (cache)
        FREE_SEQ_T(cache);
    cache = tmp;
    cache_cap = new_cap;
}

/* Test whether a cube contains all decimal digits 0‑9. */
static bool cube_has_all_digits(seq_t cube)
{
    int mask = 0;
    while (cube > 0) {
        int d = (int)(cube % 10);
        mask |= 1 << d;
        cube /= 10;
    }
    return mask == 1023; /* 0x3FF: all ten bits set */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset‑1 behaviour irrelevant for n<1 */
        return (seq_t)0;

    /* Search until we have at least n terms. */
    while (cache_size < (size_t)n) {
        ++last_candidate;
        seq_t cand = (seq_t)last_candidate;
        seq_t cube = cand * cand;    /* cand² */
        cube = cube * cand;          /* cand³ */
        if (cube_has_all_digits(cube)) {
            if (cache_size == cache_cap)
                grow_cache(cache_cap == 0 ? 16 : cache_cap * 2);
            cache[cache_size++] = cand;
        }
    }
    return cache[(size_t)n - 1];
}
