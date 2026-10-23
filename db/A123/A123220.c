/*
 * A123220  a(n)=the (1,1)-term of M^(n-1), where M=matrix(5,5,
 *          [3,-1,-1,-1,-1; 1,3,-1,-1,-1; 1,1,3,-1,-1; 1,1,1,3,-1;
 *          1,1,1,1,3]).
 * https://oeis.org/A123220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 15*a(n-1) - 100*a(n-2) + 360*a(n-3) - 680*a(n-4) +
 *            528*a(n-5) Initial values: a(1)=1, a(2)=3, a(3)=5, a(4)=-9,
 *            a(5)=-99
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 16, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <vector>
#endif

/* ------------------------------------------------------------------ */
/* Static cache to hold computed terms.  Separate implementations for C
   and C++ to correctly initialize objects of non‑trivial type seq_t
   (e.g. boost::multiprecision::cpp_int).  The cache is indexed by 1
   (a(1) stored at index 1).                                                      */
#if defined(__cplusplus)
/* C++ implementation using std::vector */
static std::vector<seq_t> cache_vec;       /* vector grows on demand       */
static idx_t cache_computed_max = 0;        /* highest index computed so far */
#else
/* C implementation using malloc/realloc */
static seq_t *cache = NULL;                /* raw array of seq_t           */
static size_t cache_cap = 0;               /* allocated size (elements)    */
static idx_t cache_computed_max = 0;        /* highest computed index       */
#endif

/* ------------------------------------------------------------------ */
/* Ensure that the cache can hold elements up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
#if defined(__cplusplus)
    if (cache_vec.size() <= (size_t)n)
        cache_vec.resize((size_t)n + 1);    /* +1 because indices start at 1 */
#else
    if ((size_t)n < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < (size_t)n + 1)   /* +1 to accommodate index n */
        new_cap <<= 1;
    void *tmp = realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* Allocation failed – terminate */
    cache = (seq_t *)tmp;
    cache_cap = new_cap;
#endif
}

/* ------------------------------------------------------------------ */
/* Initialize the first five terms once. */
static void init_first_five(void)
{
#if defined(__cplusplus)
    if (cache_vec.size() < 6)
        cache_vec.resize(6);
    cache_vec[1] = (seq_t)1;
    cache_vec[2] = (seq_t)3;
    cache_vec[3] = (seq_t)5;
    cache_vec[4] = (seq_t)-9;
    cache_vec[5] = (seq_t)-99;
    cache_computed_max = 5;
#else
    if (cache_cap < 6)
        ensure_cache(5);
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)3;
    cache[3] = (seq_t)5;
    cache[4] = (seq_t)-9;
    cache[5] = (seq_t)-99;
    cache_computed_max = 5;
#endif
}

/* ------------------------------------------------------------------ */
/* Main sequence function.  Compatible with both C and C++. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined, but return 0 safely */

    ensure_cache(n);

    if (cache_computed_max == 0)
        init_first_five();

    if (n <= cache_computed_max)
    {
#if defined(__cplusplus)
        return cache_vec[(size_t)n];
#else
        return cache[(size_t)n];
#endif
    }

    /* Compute remaining terms using the recurrence relation. */
    for (idx_t i = cache_computed_max + 1; i <= n; ++i)
    {
#if defined(__cplusplus)
        cache_vec[(size_t)i] =
            (seq_t)15  * cache_vec[(size_t)i - 1]
          - (seq_t)100 * cache_vec[(size_t)i - 2]
          + (seq_t)360 * cache_vec[(size_t)i - 3]
          - (seq_t)680 * cache_vec[(size_t)i - 4]
          + (seq_t)528 * cache_vec[(size_t)i - 5];
#else
        cache[(size_t)i] =
            (seq_t)15  * cache[(size_t)i - 1]
          - (seq_t)100 * cache[(size_t)i - 2]
          + (seq_t)360 * cache[(size_t)i - 3]
          - (seq_t)680 * cache[(size_t)i - 4]
          + (seq_t)528 * cache[(size_t)i - 5];
#endif
    }
    cache_computed_max = n;

#if defined(__cplusplus)
    return cache_vec[(size_t)n];
#else
    return cache[(size_t)n];
#endif
}
