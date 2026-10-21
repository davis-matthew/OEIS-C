/*
 * A229754  Number of defective 4-colorings of an n X 7 0..3 array connected
 *          horizontally, vertically, diagonally and antidiagonally with
 *          exactly two mistakes, and colors introduced in row-major 0..3
 *          order.
 * https://oeis.org/A229754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 20, u64 n <= 50, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A229754  
    Number of defective 4‑colorings of an n×7 0..3 array, connected in all
    eight directions, with exactly two mistakes, colors introduced in
    row‑major 0..3 order.

    Recurrence (valid for n > 11):
        a(n) = 9*a(n‑1) – 33*a(n‑2) + 63*a(n‑3) – 66*a(n‑4) + 36*a(n‑5) – 8*a(n‑6)

    Initial values (1‑based):
        a(1)=210, a(2)=20032, a(3)=41872, a(4)=69198, a(5)=113330,
        a(6)=186396, a(7)=314700, a(8)=557578, a(9)=1046550,
        a(10)=2070144, a(11)=4258696
*/

#include <stdlib.h>

/* Pre‑computed initial terms (index 0 is a dummy 0) */
static const seq_t init[12] = {
    0ULL,       /* dummy for index 0 */
    210ULL,     /* a(1) */
    20032ULL,   /* a(2) */
    41872ULL,   /* a(3) */
    69198ULL,   /* a(4) */
    113330ULL,  /* a(5) */
    186396ULL,  /* a(6) */
    314700ULL,  /* a(7) */
    557578ULL,  /* a(8) */
    1046550ULL, /* a(9) */
    2070144ULL, /* a(10) */
    4258696ULL  /* a(11) */
};

/* Recurrence coefficients */
static const seq_t C9  = (seq_t)9;
static const seq_t C33 = (seq_t)33;
static const seq_t C63 = (seq_t)63;
static const seq_t C66 = (seq_t)66;
static const seq_t C36 = (seq_t)36;
static const seq_t C8  = (seq_t)8;

/* Cache for computed sequence values */
static seq_t *cache  = NULL;  /* cache[0] is unused; cache[1] holds a(1) */
static idx_t  cached = 0;     /* highest n that has been cached */

/* Update or expand the cache to at least size n+1.
   The function is safe to call from any thread or order of calls. */
static void ensure_cache(idx_t n)
{
    if (cached >= n)            /* nothing to do */
        return;

    /* Allocate or expand the cache array */
    size_t new_size = (size_t)(n + 1);       /* include index 0 */
    seq_t *tmp;
#ifdef __cplusplus
    tmp = static_cast<seq_t*>(realloc(cache, new_size * sizeof(seq_t)));
#else
    tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
#endif
    if (!tmp)          /* soft failure – the caller will get 0 */
        return;
    cache = tmp;

    /* Compute terms that have not been computed yet */
    for (idx_t i = cached + 1; i <= n; ++i) {
        if (i <= 11) {          /* initial table */
            cache[i] = init[i];
        } else {                /* recurrence */
            seq_t pos = cache[i-1] * C9;
            pos += cache[i-3] * C63;
            pos += cache[i-5] * C36;

            seq_t neg = cache[i-2] * C33;
            neg += cache[i-4] * C66;
            neg += cache[i-6] * C8;

            cache[i] = pos - neg;
        }
    }
    cached = n;
}

seq_t a(idx_t n)
{
    if (n < 1)            /* behaviour below offset does not matter */
        return 0;

    ensure_cache(n);
    /* If memory allocation failed, cache[n] will be 0 */
    return cache ? cache[n] : 0;
}
