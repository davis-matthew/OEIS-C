/*
 * A092920  Number of strongly monotone partitions of [n].
 * https://oeis.org/A092920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the first component of v_n = M^n × [1,0,0,…], where M is
 *            the infinite tridiagonal matrix with 1’s on the super- and
 *            sub‑diagonals and diagonal entries (1,1,2,3,4,5,…). The
 *            algorithm keeps v_n incrementally and caches every computed a(n)
 *            for O(1) retrieval on future calls
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 30, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static state --------------------------------------------------- */
    static idx_t   computed = 0;   /* highest n already computed   */
    static seq_t * cache    = NULL;  /* a(0)…a(computed)            */
    static seq_t * cur      = NULL;  /* current vector v_n           */
    static seq_t * next     = NULL;  /* temporary buffer             */
    static size_t alloc    = 0;      /* size of allocated arrays     */

    /* first allocation or growth ----------------------------------- */
    if (cache == NULL) {
        alloc = (size_t)n + 2;               /* enough for [0..n+1] */
        cache = (seq_t*)malloc(alloc * sizeof(seq_t));
        cur   = (seq_t*)malloc(alloc * sizeof(seq_t));
        next  = (seq_t*)malloc(alloc * sizeof(seq_t));
        /* clear all entries */
        memset(cache, 0, alloc * sizeof(seq_t));
        memset(cur,   0, alloc * sizeof(seq_t));
        memset(next,  0, alloc * sizeof(seq_t));
        /* initialise first state */
        cache[0] = (seq_t)1;
        cur[0]   = (seq_t)1;
        computed = 0;
    } else if ((size_t)n + 1 > alloc) {
        size_t newsize = (size_t)n + 2;
        seq_t *nc = (seq_t*)realloc(cache, newsize * sizeof(seq_t));
        seq_t *nr = (seq_t*)realloc(cur,   newsize * sizeof(seq_t));
        seq_t *nn = (seq_t*)realloc(next,  newsize * sizeof(seq_t));
        cache = nc; cur = nr; next = nn;
        /* zero freshly allocated part */
        memset(cache + alloc, 0, (newsize - alloc) * sizeof(seq_t));
        memset(cur   + alloc, 0, (newsize - alloc) * sizeof(seq_t));
        memset(next  + alloc, 0, (newsize - alloc) * sizeof(seq_t));
        alloc = newsize;
    }

    /* return already computed value ---------------------------------- */
    if (n <= computed)
        return cache[n];

    /* compute successive vectors up to n ---------------------------- */
    for (idx_t step = computed; step < n; ++step) {
        size_t len = (size_t)(step + 1);   /* length of current vector */

        /* first component: cur[0]*1 + cur[1]*1 (if available) */
        next[0] = cur[0];
        if (len > 1)
            next[0] += cur[1];

        /* middle components: 1 .. len-1 */
        for (size_t i = 1; i < len; ++i) {
            seq_t diag = (i == 1 ? (seq_t)1 : (seq_t)i);   /* diagonal */
            seq_t val  = cur[i - 1] + diag * cur[i];
            if (i + 1 < len)
                val += cur[i + 1];
            next[i] = val;
        }

        /* last component: cur[len-1] (only sub‑diagonal term) */
        next[len] = cur[len - 1];

        /* swap buffers */
        seq_t *tmp = cur; cur = next; next = tmp;

        /* store new a(step+1) */
        cache[step + 1] = cur[0];
    }

    computed = n;
    return cache[n];
}
