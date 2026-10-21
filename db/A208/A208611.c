/*
 * A208611  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A208610; see the Formula section.
 * https://oeis.org/A208611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangle of coefficients of the polynomials v(n,x) defined by
 *            u(1,x)=1, v(1,x)=1 and for n>1 u(n,x)=u(n-1,x)+x*v(n-1,x)
 *            v(n,x)=(x+1)*u(n-1,x)+2x*v(n-1,x)+1 The sequence a(n) is the
 *            coefficients of v(n,x) listed row‑major, with a(1)=1, a(2)=2,
 *            a(3)=3, a(4)=2, a(5)=6, a(6)=7, …
 * Range:     u8 n <= 25, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the rows of the triangle incrementally
   and stores the sequence values in a growing cache.  It works for
   all n≥1, is memoised, and runs in O(n) time with O(n) memory. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for the sequence values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    /* arrays for the last computed row of u and v;
       maximum row length needed for n≤10000 is 141, keep extra space */
    static seq_t u[200], v[200];
    static seq_t u_next[200], v_next[200];
    static uint64_t cur_row = 1;   /* number of rows already computed */

    /* one–time initialisation */
    if (cache == NULL) {
        cache_cap = 1024;                     /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_size = 1;
        cache[0] = (seq_t)1;                  /* a(1) = 1 */
        u[0] = (seq_t)1;                      /* u(1,x) = 1 */
        v[0] = (seq_t)1;                      /* v(1,x) = 1 */
    }

    if ((size_t)n <= cache_size)
        return cache[n - 1];

    /* build rows until we have at least n values cached */
    while (cache_size < (size_t)n) {
        cur_row++;                           /* new row number */
        uint64_t r = cur_row;                /* length of new row */

        /* -- compute next row of u ---------------------------------- */
        u_next[0] = (seq_t)1;
        for (uint64_t i = 1; i < r; ++i) {
            seq_t prev_u_i  = (i < r - 1) ? u[i] : (seq_t)0;
            seq_t prev_v_im1 = v[i - 1];
            u_next[i] = prev_u_i + prev_v_im1;
        }

        /* -- compute next row of v ---------------------------------- */
        v_next[0] = (seq_t)2;                /* constant term is always 2 */
        const seq_t two = (seq_t)2;
        for (uint64_t i = 1; i < r; ++i) {
            seq_t prev_u_i   = (i < r - 1) ? u[i] : (seq_t)0;
            seq_t prev_u_im1 = (i - 1 < r - 1) ? u[i - 1] : (seq_t)0;
            seq_t prev_v_im1 = v[i - 1];
            v_next[i] = prev_u_i + prev_u_im1 + two * prev_v_im1;
        }

        /* grow the cache if necessary */
        if (cache_cap < cache_size + r) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1024;
            while (new_cap < cache_size + r)
                new_cap <<= 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* append the new row's coefficients to the cache */
        for (uint64_t i = 0; i < r; ++i)
            cache[cache_size + i] = v_next[i];
        cache_size += r;

        /* prepare for the next iteration */
        for (uint64_t i = 0; i < r; ++i) {
            u[i] = u_next[i];
            v[i] = v_next[i];
        }
    }

    return cache[n - 1];
}
