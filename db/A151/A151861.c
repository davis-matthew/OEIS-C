/*
 * A151861  a(0)=0; a(1)=1; a(2)=1; for n>=3 if n=2^i + j with 0<=j<2^i then
 *          a(n)=2*a(j) + a(j + 1) except we add 1 if j=2^i-1.
 * https://oeis.org/A151861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A151861
   a(0) = 0
   a(1) = 1
   a(2) = 1
   for n >= 3: let i be such that 2^i <= n < 2^(i+1),
   let j = n - 2^i (0 <= j < 2^i),
   then a(n) = 2*a(j) + a(j+1)   (add 1 if j == 2^i-1)
*/
#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
#  include <new>
#endif

/* Static cache ----------------------------------------------------------- */
static seq_t *cache   = NULL;   /* array holding computed values   */
static size_t cap     = 0;      /* allocated capacity (>=1 if cache!=NULL) */
static size_t computed = 0;     /* indices [0 .. computed-1] have valid values */

/* Create an element at *p with value v. ------------------------------- */
static inline void create_elem(seq_t *p, seq_t v)
{
#ifdef __cplusplus
    new (p) seq_t(v);   /* placement new in C++                */
#else
    *p = v;             /* simple assignment in C              */
#endif
}

/* Default‑construct an element at *p (no‑op for C). -------------------- */
static inline void default_elem(seq_t *p)
{
#ifdef __cplusplus
    new (p) seq_t();    /* default constructor in C++          */
#endif
}

/* Initialise cache for the first use. --------------------------------- */
static void init_cache(void)
{
    if (cache != NULL) return;          /* already initialised */

    /* need indices 0..2 */
    size_t initCap = 3;
    cache = (seq_t *)malloc(initCap * sizeof(seq_t));
    if (!cache) abort();

    /* default‑construct as needed */
    for (size_t i = 0; i < initCap; ++i) default_elem(&cache[i]);

    /* set known values */
    create_elem(&cache[0], (seq_t)0);
    create_elem(&cache[1], (seq_t)1);
    create_elem(&cache[2], (seq_t)1);

    cap = initCap;
    computed = 3;
}

/* Grow cache so that indices 0..n are valid. --------------------------- */
static void grow_cache_to(size_t n)
{
    if (cap > n) return;                /* already big enough */

    size_t newCap = cap ? cap : 1;
    while (newCap <= n) newCap <<= 1;

    seq_t *newCache = (seq_t *)malloc(newCap * sizeof(seq_t));
    if (!newCache) abort();

    /* copy existing values */
    if (cache) memcpy(newCache, cache, cap * sizeof(seq_t));

    /* default‑construct new elements */
    for (size_t i = cap; i < newCap; ++i) default_elem(&newCache[i]);

    free(cache);
    cache = newCache;
    cap   = newCap;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_cache();                     /* ensure base values exist   */

    /* If already computed, return it directly. */
    if ((size_t)n < computed) return cache[(size_t)n];

    // grow cache to contain all indices up to n
    grow_cache_to((size_t)n);

    /* Compute all missing terms up to n. */
    for (idx_t k = (idx_t)computed; k <= n; ++k)
    {
        /* highest power of two <= k */
        idx_t pow2 = 1;
        while (pow2 <= (k >> 1)) pow2 <<= 1;   /* avoids overflow */

        idx_t j = k - pow2;          /* 0 ≤ j < pow2          */

        seq_t val = (seq_t)2 * cache[(size_t)j] + cache[(size_t)(j + 1)];
        if (j == pow2 - 1) val += (seq_t)1;

        cache[(size_t)k] = val;
    }

    computed = (size_t)n + 1;
    return cache[(size_t)n];
}
