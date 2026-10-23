/*
 * A121553  Total area of all deco polyominoes of height n. A deco polyomino
 *          is a directed column-convex polyomino in which the height,
 *          measured along the diagonal, is attained only in the last column.
 * https://oeis.org/A121553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n)=n*a(n-1)+(n-1)!*(1+n(n-1)/2) for n>=2 * which is
 *            equivalent to a(n)=n!*[n(n-1)/4+1/1+1/2+...+1/n]
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *fact_cache = NULL; /* factorial values fact[i]=i!      */
static seq_t *a_cache   = NULL; /* a(n) values                     */
static size_t cache_size = 0;    /* number of elements currently  */
static size_t cache_cap  = 0;    /* allocated capacity              */

static void
grow_cache_to(idx_t n)
{
    /* Ensure capacity for indices 0..n */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap? cache_cap*2 : 8;
        while (new_cap <= (size_t)n) new_cap *= 2;
        fact_cache = (seq_t *)realloc(fact_cache, new_cap * sizeof(seq_t));
        a_cache   = (seq_t *)realloc(a_cache,   new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialise base cases if not yet done */
    if (cache_size == 0) {
        fact_cache[0] = (seq_t)1;          /* 0! = 1   */
        a_cache[0]   = (seq_t)0;           /* unused   */
        cache_size   = 1;
    }
    if (cache_size == 1) {                /* now 0 present, compute for n=1    */
        fact_cache[1] = (seq_t)1;          /* 1! = 1   */
        a_cache[1]   = (seq_t)1;           /* a(1)=1   */
        cache_size   = 2;
    }

    /* Compute missing values up to n */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        /* factorial */
        fact_cache[i] = fact_cache[i-1] * (seq_t)i;

        /* a(i) = i*a(i-1) + (i-1)! * (1 + i*(i-1)/2) */
        seq_t term1 = (seq_t)i * a_cache[i-1];

        seq_t middle = (seq_t)i * (i-1) / (seq_t)2; /* i*(i-1)/2 */
        middle += (seq_t)1;                         /* +1        */

        seq_t term2 = fact_cache[i-1] * middle;

        a_cache[i] = term1 + term2;
    }
    cache_size = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* offset safety */
    grow_cache_to(n);
    return a_cache[n];
}
