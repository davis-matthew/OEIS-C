/*
 * A170868  a(0) = 1, a(1) = 2; a(2^i + j) = 3a(j) + a(j + 1) for 0 <= j <
 *          2^i.
 * https://oeis.org/A170868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2; a(2^i + j) = 3*a(j)+a(j+1) for 0 <= j < 2^i
 * Range:     u8 n <= 28, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoised computation of a(n).  All values are stored in a dynamically
   resized static array.  The array is initialised to zero, which is a
   sentinel value because the sequence never contains zero. */
static seq_t *cache = NULL;
static size_t cap = 0;          /* number of slots allocated in cache */

/* Ensure the cache can hold values up to index n (inclusive).  New
   entries are cleared to zero. */
static void ensure_cache(idx_t n)
{
    if (cache && n + 1 <= cap)
        return;

    size_t new_cap = cap ? cap * 2 : 16;          /* start small */
    while (n + 1 > new_cap)
        new_cap *= 2;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();                      /* allocation failure */

    if (cache) {
        /* Zero the new region only */
        size_t added = new_cap - cap;
        for (size_t i = 0; i < added; ++i)
            new_cache[cap + i] = (seq_t)0;
    } else {
        /* First allocation: clear the whole array */
        for (size_t i = 0; i < new_cap; ++i)
            new_cache[i] = (seq_t)0;
    }

    cache = new_cache;
    cap = new_cap;
}

/* Recursive calculation with memoisation.  The cache is queried and
   stored before recursing. */
static seq_t calc(idx_t n)
{
    ensure_cache(n);

    seq_t val = cache[n];
    if (val != (seq_t)0)
        return val;                       /* already computed */

    seq_t result;
    if (n == 0)
        result = (seq_t)1;
    else if (n == 1)
        result = (seq_t)2;
    else {
        /* Find the largest power of two <= n */
        idx_t pow2 = 1ULL << (63 - __builtin_clzll(n));
        idx_t j = n - pow2;
        seq_t aj  = calc(j);
        seq_t aj1 = calc(j + 1);
        result = (seq_t)3 * aj + aj1;
    }

    cache[n] = result;
    return result;
}

/* Public interface that the tests will call. */
seq_t a(idx_t n)
{
    return calc(n);
}
