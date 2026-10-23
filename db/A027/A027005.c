/*
 * A027005  a(n) = T(2*n+1,n+2), T given by A026998.
 * https://oeis.org/A027005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(2*n+5) - (6*n^2 + 11*n + 11) where Lucas(0)=2,
 *            Lucas(1)=1, Lucas(k)=Lucas(k-1)+Lucas(k-2)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 43, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for Lucas numbers: lucas_cache[i] = L(i)  */
static seq_t *lucas_cache = NULL;      /* array of Lucas numbers   */
static idx_t lucas_cache_size = 0;     /* number of entries filled */
static idx_t lucas_cache_alloc = 0;    /* allocated capacity       */

/* Ensure that Lucas numbers up to index k are computed and cached. */
static void
ensure_lucas(idx_t k)
{
    /* If already computed up to k, nothing to do */
    if (lucas_cache && lucas_cache_size > k)
        return;

    /* Determine new capacity needed */
    idx_t new_alloc = lucas_cache_alloc ? lucas_cache_alloc : 16;
    idx_t new_size  = k + 1;
    while (new_alloc < new_size)
        new_alloc <<= 1;     /* grow by factor 2 */

    /* Allocate or reallocate the cache */
    if (!lucas_cache) {
        lucas_cache = (seq_t*)malloc(new_alloc * sizeof(seq_t));
        if (!lucas_cache) abort();          /* fatal error */
        /* Initialise first two Lucas numbers */
        lucas_cache[0] = (seq_t)2;
        lucas_cache[1] = (seq_t)1;
        lucas_cache_size = 2;
    } else if (lucas_cache_alloc < new_alloc) {
        seq_t *tmp = (seq_t*)realloc(lucas_cache, new_alloc * sizeof(seq_t));
        if (!tmp) abort();                  /* fatal error */
        lucas_cache = tmp;
        lucas_cache_alloc = new_alloc;
    }

    /* Compute missing Lucas values up to index k */
    for (idx_t i = lucas_cache_size; i <= k; ++i)
        lucas_cache[i] = lucas_cache[i-1] + lucas_cache[i-2];
    lucas_cache_size = k + 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;      /* undefined for n < 1, but harmless */

    /* Compute required Lucas index: L(2*n+5) */
    idx_t k = 2 * n + 5;
    ensure_lucas(k);

    /* Compute the polynomial part: 6*n^2 + 11*n + 11 */
    seq_t sub = (seq_t)6 * n * n + (seq_t)11 * n + 11;

    /* Return Lucas(k) - sub */
    return lucas_cache[k] - sub;
}
