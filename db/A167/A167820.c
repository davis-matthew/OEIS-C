/*
 * A167820  Subsequence of A167709 whose indices are congruent to 0 mod 5,
 *          i.e., a(n) = A167709(5*n).
 * https://oeis.org/A167820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167820: a(0)=0, a(1)=351, a(n+2)=340*a(n+1)-a(n) */
#include <stdlib.h>

/* Computes the n-th term of sequence A167820.
 * The function is memoised so it can be called in any order;
 * the cache is grown as needed. */
seq_t a(idx_t n)
{
    /* static cache and bookkeeping */
    static seq_t *cache = NULL;          /* array of computed terms   */
    static size_t  cap   = 0;            /* current capacity          */
    static size_t  last  = 0;            /* highest index computed    */

    /* initialise base cases if this is the first call */
    if (cache == NULL) {
        /* allocate space for the first two terms */
        cache = (seq_t *)calloc(2, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)0;                        /* a(0) */
        cache[1] = (seq_t)351;                       /* a(1) */
        cap  = 2;
        last = 1;                                   /* we have a(0) and a(1) */
    }

    /* grow the cache if n is beyond the current capacity */
    if ((size_t)n + 1 > cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;            /* allocation failed */
        cache = new_cache;
        cap   = (size_t)n + 1;
    }

    /* compute missing terms up to n using the recurrence */
    size_t start = last + 1;
    if (start < 2) start = 2;                     /* skip over already known a(0) and a(1) */

    for (size_t i = start; i <= (size_t)n; ++i) {
        /*  unsigned overflow is defined modulo 2^bits, which is exactly what we need  */
        cache[i] = cache[i - 1] * (seq_t)340 - cache[i - 2];
    }
    last = (size_t)n;

    return cache[(size_t)n];
}
