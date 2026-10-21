/*
 * A051292  Whitney number of level n of the lattice of the ideals of the
 *          crown of size 2 n.
 * https://oeis.org/A051292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 49, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A051292: Whitney number of level n of the lattice of the ideals of the crown of size 2 n
   Recurrence:
        n*(n-3)*a(n) - (2n-1)(n-3)*a(n-1) + (-n²+4n-5)*a(n-2)
          - (n-1)(2n-7)*a(n-3) + (n-1)(n-4)*a(n-4) = 0
   with a(0)=2, a(1)=1, a(2)=1, a(3)=4, a(4)=9.
   The routine memoises values so that calls may be out of order.
   For 64‑bit seq_t it uses 128‑bit intermediates to avoid overflow.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#if defined(__SIZEOF_INT128__)
typedef __int128 int128_t;
#define HAVE_INT128 1
#else
#define HAVE_INT128 0
#endif

/* static cache of computed terms   */
static seq_t   *cache   = NULL;   /* array of seq_t values */
static idx_t   cache_size = 0;    /* allocated size   */
static idx_t   last_computed = 0; /* highest n computed */

/* ensure cache can hold up to n */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;           /* already big enough */
    idx_t new_size = cache_size ? cache_size * 2 : 5;
    if (new_size <= n) new_size = n + 1;   /* at least n+1 */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return;                     /* allocation failure ignored */
    cache = tmp;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (!cache) {
        cache_size = 5;                    /* Need indices 0..4          */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* failure, return 0          */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)9;
        last_computed = 4;
    }

    if (n <= last_computed)            /* already computed */
        return cache[n];

    ensure_cache(n);                    /* grow if necessary */

    /* compute subsequent terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t result;
        /* coefficients used in the recurrence */
        uint64_t c1 = (2 * i - 1) * (i - 3);          /* (2i-1)(i-3)    */
        uint64_t c2 = (i * i) - 4 * i + 5;           /*  i^2-4i+5     */
        uint64_t c3 = (i - 1) * (2 * i - 7);         /* (i-1)(2i-7)    */
        uint64_t c4 = (i - 1) * (i - 4);             /* (i-1)(i-4)     */
        uint64_t denom = i * (i - 3);                /* i*(i-3)        */

#if HAVE_INT128
        /* 64‑bit seq_t case: use 128‑bit intermediate */
        if (sizeof(seq_t) <= 8) {
            int128_t num = 0;
            num += (__int128)c1 * (int128_t)cache[i-1];
            num += (__int128)c2 * (int128_t)cache[i-2];
            num += (__int128)c3 * (int128_t)cache[i-3];
            num -= (__int128)c4 * (int128_t)cache[i-4];
            result = (seq_t)(num / denom);          /* exact division */
        } else {
#endif
            /* arbitrary‑precision or larger builtin type */
            seq_t term1 = cache[i-1] * (seq_t)c1;
            seq_t term2 = cache[i-2] * (seq_t)c2;
            seq_t term3 = cache[i-3] * (seq_t)c3;
            seq_t term4 = cache[i-4] * (seq_t)c4;
            seq_t num = term1 + term2 + term3 - term4;
            seq_t den = (seq_t)i * (seq_t)(i - 3);
            result = num / den;   /* exact division; built‑in seq_t supports it */
#if HAVE_INT128
        }
#endif
        cache[i] = result;
    }
    last_computed = n;
    return cache[n];
}
