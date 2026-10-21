/*
 * A081985  Hypotenuses of primitive Pythagorean triangles with even short leg
 *          (or odd long leg) sorted on semiperimeter.
 * https://oeis.org/A081985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A081985
 *
 *  Hypotenuses of primitive Pythagorean triangles with even short leg
 *  (short leg 2*u*v is smaller than the long leg u^2-v^2) sorted by
 *  semiperimeter s = u*(u+v).
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd_u64.h"

/* local data structure for temporary sorting */
typedef struct { seq_t s; seq_t hyp; } term_t;

/* global cache of sorted hypotenuses */
static seq_t *hyp_cache     = NULL;   /* array of hypotenuses, sorted by semiperimeter */
static size_t cache_terms   = 0;       /* number of terms stored */

/* gcd for uint64_t used in the generation step */

/* comparison function for sorting by semiperimeter, then hypotenuse */
static int cmp_terms(const void *p1, const void *p2)
{
    const term_t *a = (const term_t *)p1;
    const term_t *b = (const term_t *)p2;
    if (a->s < b->s) return -1;
    if (a->s > b->s) return  1;
    if (a->hyp < b->hyp) return -1;
    if (a->hyp > b->hyp) return  1;
    return 0;
}

/* rebuild the cache to contain at least n terms.
 * u_limit is increased in powers of two until we have enough terms. */
static void ensure_terms(idx_t n)
{
    static idx_t current_u_limit = 2000;   /* starting bound */
    if (cache_terms >= (size_t)n) return;  /* already enough */

    /* free old cache if present */
    if (hyp_cache) { free(hyp_cache); hyp_cache = NULL; }
    cache_terms = 0;

    while (1) {
        /* temporary buffer for collected pairs */
        size_t cap = 0, cnt = 0;
        term_t *tmp = NULL;

        for (uint64_t u = 1; u <= current_u_limit; ++u) {
            for (uint64_t v = 1; v < u; ++v) {
                /* require opposite parity and coprimality */
                if (((u - v) & 1ULL) == 0) continue;
                if (gcd_u64(u, v) != 1) continue;

                /* short leg must be the smaller of the two legs */
                uint64_t two_uv   = 2ULL * u * v;
                uint64_t diff_sq  = u * u - v * v;
                if (two_uv >= diff_sq) continue;            /* not short enough */

                /* store the pair (semiperimeter, hypotenuse) */
                if (cnt == cap) {
                    size_t new_cap = cap ? (cap << 1) : 16;
                    term_t *p = (term_t *)realloc(tmp, new_cap * sizeof(term_t));
                    if (!p) { free(tmp); exit(1); }
                    tmp = p; cap = new_cap;
                }
                tmp[cnt].s   = (seq_t)u * ( (seq_t)u + (seq_t)v );
                tmp[cnt].hyp = (seq_t)u * (seq_t)u + (seq_t)v * (seq_t)v;
                ++cnt;
            }
        }

        /* sort the collected pairs by semiperimeter, then hypotenuse */
        qsort(tmp, cnt, sizeof(term_t), cmp_terms);

        /* allocate the hypotenuse array */
        hyp_cache = (seq_t *)malloc(cnt * sizeof(seq_t));
        if (!hyp_cache) { free(tmp); exit(1); }
        for (size_t i = 0; i < cnt; ++i) hyp_cache[i] = tmp[i].hyp;
        free(tmp);

        cache_terms = cnt;
        if (cache_terms >= (size_t)n) break;

        /* not enough terms – increase the bound and try again */
        current_u_limit <<= 1;  /* double the search limit */
    }
}

/* required function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                     /* undefined for n < 1 */
    ensure_terms(n);
    return hyp_cache[(size_t)(n - 1)];
}
