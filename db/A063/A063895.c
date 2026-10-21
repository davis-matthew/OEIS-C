/*
 * A063895  Start with x, xy; then concatenate each word in turn with all
 *          preceding words, getting x xy xxy xxxy xyxxy xxxxy xyxxxy xxyxxxy
 *          ...; sequence gives number of words of length n. Also binary trees
 *          by degree: x (x,y) (x,(x,y)) (x,(x,(x,y))) ((x,y),(x,(x,y)))...
 * https://oeis.org/A063895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice, eigen
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 19, u32 n <= 33, u64 n <= 61, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (OEIS A063895):
 *   a(1) = a(2) = 1;
 *   for n > 2:
 *     a(n) = Σ_{i=1}^{⌊(n‑1)/2⌋} a(i) * a(n‑i)
 *            + (n even ? (a(n/2)‑1) * a(n/2) / 2 : 0)  */
#include <stdlib.h>

/*-------------------------  cache support  ------------------------*/
static seq_t  *cache     = NULL;   /* cache[1] ... cache[n], cache[0] unused      */
static idx_t   cache_cap = 0;      /* allocated capacity (number of slots)      */
static idx_t   cache_end = 0;      /* highest computed index (>= 0)             */

/* Ensure the cache contains entries 1..n (inclusive).  */
static void ensure_cache(idx_t n)
{
    /* Allocate/expand cache if necessary. */
    if (cache == NULL || cache_cap <= n) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 4;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the base cases once. */
    if (cache_end == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache_end = 2;
    }

    /* Compute missing values up to n. */
    for (idx_t m = cache_end + 1; m <= n; ++m) {
        seq_t sum = 0;
        /* Sum over i < j with i + j = m. */
        for (idx_t i = 1; i < m - i; ++i) {
            sum += cache[i] * cache[m - i];
        }
        /* Add the special even‑index term if applicable. */
        if ((m & 1) == 0) {
            idx_t k = m >> 1;
            seq_t ak = cache[k];
            sum += (ak - (seq_t)1) * ak / (seq_t)2;
        }
        cache[m] = sum;
    }
    cache_end = n;
}

/* --------------  public sequence function  -------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for offset 1 */
    if (n <= 2) return (seq_t)1;
    ensure_cache(n);
    return cache[n];
}
