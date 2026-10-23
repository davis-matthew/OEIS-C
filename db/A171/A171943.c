/*
 * A171943  To get a(n+1), add to a(n) the product of the two closest digits
 *          to the comma printed before a(n); start with a(1)=a(2)=1.
 * https://oeis.org/A171943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 6092, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A171943: a(1)=a(2)=1, a(n)=a(n−1)+lsd(a(n−2))*msd(a(n−1)) for n≥3,
   where lsd(x)=x mod 10 and msd(x) is the most significant decimal digit of x. */
/* The code works in C11 and C++17.  For C++ builds seq_t can be an arbitrary‑precision
   integer class; for C builds seq_t is an unsigned integer type. */

#include <stdlib.h>   /* malloc, free, exit */
#include <stddef.h>   /* size_t */

/* Helper: most significant decimal digit of x.  Works for both integral
   and arbitrary‑precision seq_t (assumes operator/ and operator>= are defined). */
static seq_t msd(seq_t x)
{
    while (x >= 10)
        x /= 10;
    return x;
}

/* ------------ Memory handling ---------------------------------------- */
static seq_t *cache = NULL;            /* array of terms, 1‑based */
static idx_t cache_len = 0;            /* next free index (index of first unused entry) */
static size_t cache_cap = 0;           /* capacity in number of elements */

/* Expand the cache to hold at least 'needed' entries (1‑based).  For C++
   we use new[]/delete[] which properly constructs/destructs seq_t objects.
   For C we fall back to malloc/realloc. */
static void ensure_cache(idx_t needed)
{
    if ((idx_t)cache_cap >= needed)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while ((idx_t)new_cap < needed)
        new_cap *= 2;

#ifdef __cplusplus
    /* C++: allocate a new array and copy existing values. */
    seq_t *new_cache = new seq_t[new_cap];
    if (cache) {
        for (size_t i = 1; i < cache_len; ++i)
            new_cache[i] = cache[i];
        delete [] cache;
    }
    cache = new_cache;
#else
    /* C: use realloc; seq_t is a POD type. */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache && new_cap > 0) /* allocation failure */
        exit(1);
    cache = new_cache;
#endif
    cache_cap = new_cap;
}

/* ------------ Sequence computation ----------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined, but guard against underflow */

    /* Always ensure room for n.  For n=1,2 we will fill the cache later. */
    ensure_cache((idx_t)n + 1);

    /* initialise the first two terms on first call */
    if (cache_len <= 2) {
        cache[1] = 1;
        cache[2] = 1;
        cache_len = 3;    /* next free index is 3 */
    }

    /* Compute missing terms up to n. */
    for (idx_t idx = cache_len; idx <= n; ++idx) {
        seq_t lsd = cache[idx - 2] % 10;   /* least significant digit of a(n-2) */
        seq_t m   = msd(cache[idx - 1]);   /* most significant digit of a(n-1) */
        cache[idx] = cache[idx - 1] + lsd * m;
    }
    cache_len = n + 1;

    return cache[n];
}
