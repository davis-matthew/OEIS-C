/*
 * A105480  Number of partitions of {1...n} containing 3 pairs of consecutive
 *          integers, where each pair is counted within a block and a string
 *          of more than 2 consecutive integers are counted two at a time.
 * https://oeis.org/A105480
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n-1,3) · Bell(n-4) (offset 4)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   small_free, small_malloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/small_free.h"
#include "../utils/small_malloc.h"

/* ------------------------------------------------------------------
 *  Internal helper: compute Bell numbers up to n (inclusive) using a
 *  folding Bell triangle.  The implementation keeps a running cache
 *  so that each Bell number is computed only once.
 * ------------------------------------------------------------------ */
static seq_t *bell_cache   = NULL;   /* bell_cache[k] = Bell(k)   */
static idx_t  bell_cache_len = 0;    /* number of cached values   */
static seq_t *stir_old       = NULL; /* previous Stirling row      */
static idx_t  stir_old_len   = 0;    /* length of previous row    */

static void ensure_bell(idx_t n)
{
    if (n < bell_cache_len) return;              /* already computed      */

    /* initialise caches */
    if (bell_cache == NULL) {
        bell_cache_len = 1;
        bell_cache = (seq_t *)malloc(sizeof(seq_t));
        bell_cache[0] = (seq_t)1;                /* Bell(0) = 1 */
    }
    if (stir_old == NULL) {
        stir_old_len = 1;
        stir_old = (seq_t *)malloc(sizeof(seq_t));
        stir_old[0] = (seq_t)1;                  /* S(0,0) = 1 */
    }

    for (idx_t m = bell_cache_len; m <= n; ++m) {
        /* Allocate new Stirling row (size m+1)   */
        idx_t sz = m + 1;
        seq_t *stir_new = (seq_t *)malloc(sizeof(seq_t) * sz);
        stir_new[0] = (seq_t)0;                 /* S(m,0) = 0 for m>0 */

        /* Inner recurrence: S(m,k) = S(m-1,k-1) + k·S(m-1,k) */
        for (idx_t k = 1; k < m; ++k) {
            seq_t term1 = stir_old[k-1];
            seq_t term2 = stir_old[k];
            if (term2) term2 *= (seq_t)k;
            stir_new[k] = term1 + term2;
        }
        stir_new[m] = stir_old[m-1];            /* last element */

        /* Bell(m) = sum_{k=1..m} S(m,k)          */
        seq_t bell_val = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k) bell_val += stir_new[k];

        /* store Bell(m)   */
        seq_t *new_cache = (seq_t *)realloc(bell_cache,
                                           sizeof(seq_t) * (sz));
        bell_cache = new_cache;
        bell_cache[m] = bell_val;

        /* update Stirling row pointers           */
        small_free(stir_old);
        stir_old = stir_new;
        stir_old_len = sz;
    }
    bell_cache_len = n + 1;
}

/* ------------------------------------------------------------------
 *  Public sequence function
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;                /* undefined below offset */

    /* binomial(n-1,3) = (n-1)(n-2)(n-3)/6  */
    seq_t bm3 = (seq_t)(n-1) * (n-2) * (n-3);
    bm3 /= (seq_t)6;

    /* Bell(n-4)  */
    idx_t bIdx = n - 4;
    ensure_bell(bIdx);
    seq_t bell_val = bell_cache[bIdx];

    return bm3 * bell_val;
}
