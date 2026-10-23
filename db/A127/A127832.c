/*
 * A127832  Array T by antidiagonals: for h>=0 and k>=0, T(h,k)=number of UR
 *          paths from (0,0) to (h,k) that touch the line y=2x/3 only at
 *          lattice points. A UR path is a path of steps of length 1 each
 *          directed up or right.
 * https://oeis.org/A127832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of UR paths from (0,0) to (h,k) that touch * the
 *            line y = 2x/3 only at lattice points, where (h,k) is the *
 *            position obtained by reading the array T(h,k) by antidiagonals.
 *            * The array T is defined for h, k ≥ 0 by the recurrence: *
 *            T(h,0) = T(0,k) = 1 (except T(0,0)=0) * T(h,k) = (st...
 * Range:     u8 n <= 82, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   vertical_allowed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/vertical_allowed.h"

/* check whether a horizontal step into (h,k) would cross the line
 * at a non‑lattice point.  The step is from (h-1,k) to (h,k). */
static inline bool horizontal_allowed(idx_t h, idx_t k)
{
    /* Avoid the trivial no‑step case */
    if (h == 0) return true;   /* (0,k) never crosses the line away from start */
    /* Compute 3*k < 2h ? (mid is 3*k) */
    idx_t lhs = (h - 1) * 2;          /* 2(h-1) */
    idx_t rhs = h * 2;                /* 2h    */
    idx_t mid = 3 * k;                /* 3k    */
    /* Intersection inside the segment and not at integer x? */
    if (lhs < mid && mid < rhs && (mid & 1u)) return false; /* forbidden */
    return true;                    /* allowed */
}

/* Convert sequence index n (n ≥ 1) to coordinates (h,k) read along
 * antidiagonals: first (0,0), then (0,1),(1,0), then (0,2),(1,1),(2,0), etc. */
static inline void index_to_coords(idx_t n, idx_t *p_h, idx_t *p_k)
{
    /* Find minimal s such that triangular number T_{s+1} = (s+1)(s+2)/2 ≥ n. */
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 < n)
        ++s;
    idx_t prev = (s * (s + 1)) / 2;
    idx_t pos = n - prev - 1;         /* 0‑based position on diagonal s */
    *p_h = pos;
    *p_k = s - pos;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined below offset */

    /* coordinates */
    idx_t h, k;
    index_to_coords(n, &h, &k);
    idx_t sum = h + k;                 /* maximum diagonal needed */

    /* size of the square table */
    size_t width = (size_t)(sum + 1);

    /* allocate table, zero‑initialized */
    seq_t *tab = (seq_t *)calloc((size_t)width * width, sizeof(seq_t));
    if (!tab) return (seq_t)0;          /* allocation failure */

    /* base case: T(0,0) = 0 (already zero) */
    /* set edges: T(i,0)=T(0,j)=1 for i,j>0 */
    for (idx_t i = 1; i <= sum; ++i) {
        tab[ (size_t)i * width + 0 ] = 1;          /* T(i,0) */
        tab[ 0 * width + (size_t)i ]         = 1;  /* T(0,i) */
    }

    /* dynamic programming over interior cells */
    for (idx_t s = 2; s <= sum; ++s) {
        for (idx_t i = 1; i < s; ++i) {            /* i = h, j = k */
            idx_t j = s - i;
            seq_t val = 0;
            if (horizontal_allowed(i, j))
                val += tab[(size_t)(i - 1) * width + j];
            if (vertical_allowed(i, j))
                val += tab[(size_t)i * width + (j - 1)];
            tab[(size_t)i * width + j] = val;
        }
    }

    seq_t result = tab[(size_t)h * width + k];
    free(tab);
    return result;
}
