/*
 * A221394  Number of n X 3 arrays of occupancy after each element stays put
 *          or moves to some horizontal, vertical or antidiagonal neighbor,
 *          with every occupancy equal to zero or two.
 * https://oeis.org/A221394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A221394 – Number of n×3 arrays of occupancy after each element stays
 * put or moves to some horizontal, vertical or antidiagonal neighbour,
 * with every occupancy equal to zero or two.
 *
 * Recurrence (valid for all n≥1):
 *   a(n) = 48⋅a(n‑2) − 440⋅a(n‑4) + 998⋅a(n‑6) − 873⋅a(n‑8)
 *          + 252⋅a(n‑10) + 16⋅a(n‑12)
 *
 * Initial values (for 1 ≤ n ≤ 12):
 *   a(1)=0, a(2)=18, a(3)=0, a(4)=631, a(5)=0, a(6)=23188,
 *   a(7)=0, a(8)=852298, a(9)=0, a(10)=31322020, a(11)=0,
 *   a(12)=1151041183.
 *
 * The function memoises results in a static cache so that repeated
 * calls are O(1) after the first computation.
 */

#include <stdlib.h>

/* --------------------------------------------------------------------- */
/* Static cache storage                                            */
/* --------------------------------------------------------------------- */
static seq_t *cache    = NULL;   /* pointer to array a(1), a(2), ...           */
static idx_t  cache_sz = 0;      /* number of cached terms (>=0)               */
static idx_t  cache_cap = 0;     /* allocated capacity (>= cache_sz)           */

/* Increase cache capacity so it can hold at least 'need' elements. */
static void grow_cache(idx_t need)
{
    if (need <= cache_cap) return;          /* already sufficient */

    idx_t new_cap = (cache_cap == 0) ? 16 : cache_cap;
    while (new_cap < need) new_cap <<= 1;    /* double until large enough */

    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);                 /* memory allocation failed */

    cache = new_cache;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Initial values for 1 ≤ n ≤ 12                                          */
/* --------------------------------------------------------------------- */
static const seq_t init_vals[12] = {
    (seq_t)0,            /* a(1) */
    (seq_t)18,           /* a(2) */
    (seq_t)0,            /* a(3) */
    (seq_t)631,          /* a(4) */
    (seq_t)0,            /* a(5) */
    (seq_t)23188,        /* a(6) */
    (seq_t)0,            /* a(7) */
    (seq_t)852298,       /* a(8) */
    (seq_t)0,            /* a(9) */
    (seq_t)31322020,     /* a(10) */
    (seq_t)0,            /* a(11) */
    (seq_t)1151041183    /* a(12) */
};

/* --------------------------------------------------------------------- */
/* Main sequence function                                             */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                 /* undefined below offset */

    /* Ensure cache can hold up to n */
    grow_cache(n);

    /* If we haven't computed up to n yet, compute from cache_sz+1 up to n */
    if (cache_sz < n) {
        for (idx_t i = cache_sz + 1; i <= n; ++i) {
            seq_t val;
            if (i & 1) {                        /* odd indices are zero */
                val = (seq_t)0;
            } else if (i <= 12) {               /* use stored initial values */
                val = init_vals[i - 1];
            } else {
                /* a(i) = 48*a(i‑2) − 440*a(i‑4) + 998*a(i‑6)
                 *       − 873*a(i‑8) + 252*a(i‑10) + 16*a(i‑12) */
                val =  (seq_t)48 * cache[i - 3]   /* a(i‑2) */
                    - (seq_t)440 * cache[i - 5]   /* a(i‑4) */
                    + (seq_t)998 * cache[i - 7]   /* a(i‑6) */
                    - (seq_t)873 * cache[i - 9]   /* a(i‑8) */
                    + (seq_t)252 * cache[i - 11]  /* a(i‑10) */
                    + (seq_t)16  * cache[i - 13]; /* a(i‑12) */
            }
            cache[i - 1] = val;                  /* store into cache */
        }
        cache_sz = n;
    }

    return cache[n - 1];
}
