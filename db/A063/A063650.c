/*
 * A063650  Number of ways to tile a 6 X n rectangle with 1 X 1 and 2 X 2
 *          tiles.
 * https://oeis.org/A063650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 16*a(n-2) + a(n-3) - 27*a(n-4) + a(n-5) +
 *            4*a(n-6) Initial values: a(0)=1, a(1)=1, a(2)=13, a(3)=43,
 *            a(4)=269, a(5)=1213 This implementation keeps a static cache so
 *            that calls are answered in O(1) after the first expansion
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cast sizeof(seq_t) correctly in C++ */
#ifdef __cplusplus
#  define SEQ_SIZE sizeof(seq_t)
#else
#  define SEQ_SIZE sizeof(seq_t)
#endif

static seq_t *seq_cache = NULL;      /* array of computed terms */
static idx_t cache_size   = 0;        /* number of entries that contain valid data */
static idx_t cache_cap    = 0;        /* allocated capacity of seq_cache */

/* Grow the cache to hold at least new_cap elements */
static void grow_cache(idx_t new_cap)
{
    if (new_cap <= cache_cap) return;

#ifdef __cplusplus
    /* C++: allocate new array, copy old values, delete old array */
    seq_t *new_cache = new seq_t[new_cap];
    for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = seq_cache[i];
    delete[] seq_cache;
    seq_cache = new_cache;
#else
    /* C: use realloc (malloc for first allocation) */
    void *p = (cache_cap == 0
               ? malloc(SEQ_SIZE * new_cap)
               : realloc(seq_cache, SEQ_SIZE * new_cap));
    if (!p) exit(1);               /* allocation failed */
    seq_cache = (seq_t *)p;
#endif

    cache_cap = new_cap;
}

/* The sequence function itself */
seq_t a(idx_t n)
{
    /* Directly return if already cached */
    if (n < cache_size) return seq_cache[n];

    /* Ensure capacity for at least n+1 entries */
    if (n + 1 > cache_cap) grow_cache((n + 1) * 2);   /* double growth */

    /* Compute missing terms starting from current cache_size */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        if (i == 0)          seq_cache[i] = (seq_t)1;
        else if (i == 1)     seq_cache[i] = (seq_t)1;
        else if (i == 2)     seq_cache[i] = (seq_t)13;
        else if (i == 3)     seq_cache[i] = (seq_t)43;
        else if (i == 4)     seq_cache[i] = (seq_t)269;
        else if (i == 5)     seq_cache[i] = (seq_t)1213;
        else
        {
            seq_t term = seq_cache[i-1] * 2
                       + seq_cache[i-2] * 16
                       + seq_cache[i-3]
                       - seq_cache[i-4] * 27
                       + seq_cache[i-5]
                       + seq_cache[i-6] * 4;
            seq_cache[i] = term;
        }
    }

    cache_size = n + 1;
    return seq_cache[n];
}
