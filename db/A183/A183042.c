/*
 * A183042  Least number of knight's moves from (0,0) to the segment of points
 *          (0,n), (1,n-1), ..., (n,0) on infinite chessboard.
 * https://oeis.org/A183042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0}^{n} T(i,n-i), where T(i,j) is the minimal number
 *            of knight moves from (0,0) to the point (i,j) on an infinite
 *            chessboard
 * Range:     u8 n <= 24, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdint.h>

/* ----- helper: minimal knight distance on the infinite board ----- */
static seq_t knight_dist(idx_t x, idx_t y)
{
    /* Cast to seq_t for all computations */
    seq_t sx = (seq_t)x;
    seq_t sy = (seq_t)y;

    /* Ensure sx ≥ sy by swapping if necessary */
    if (sx < sy) {
        seq_t tmp = sx; sx = sy; sy = tmp;
    }

    /* Special cases */
    if (sx == (seq_t)1 && sy == (seq_t)0) return (seq_t)3;
    if (sx == (seq_t)2 && sy == (seq_t)2) return (seq_t)4;

    /* Core formula: max(ceil(sx/2), ceil((sx+sy)/3)) rounded up to match parity */
    seq_t d1 = (sx + (seq_t)1) / (seq_t)2;          /* ceil(sx/2) */
    seq_t d2 = (sx + sy + (seq_t)2) / (seq_t)3;    /* ceil((sx+sy)/3) */
    seq_t d  = (d1 > d2) ? d1 : d2;

    /* Adjust parity */
    if ( (d & (seq_t)1) != ((sx + sy) & (seq_t)1) )
        d++;

    return d;
}

/* ----- cache for a(n) values ----- */
static seq_t *cache = NULL;   /* array of computed a(n) */
static idx_t  cache_len = 0;  /* current length (next free index) */

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    /* Expand cache if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;

        /* Allocate or reallocate memory */
        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();  /* allocation failure */
        cache = new_cache;

        /* Compute a(i) for all i in [cache_len, new_len) */
        for (idx_t i = cache_len; i < new_len; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t j = (idx_t)0; j <= i; ++j) {
                sum += knight_dist(j, i - j);
            }
            cache[i] = sum;
        }
        cache_len = new_len;
    }

    return cache[n];
}
