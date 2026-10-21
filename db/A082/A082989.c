/*
 * A082989  Number of ordered trees with n edges and having no root-to-leaf
 *          branches.
 * https://oeis.org/A082989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A082989 (ordered trees with no root‑to‑leaf branches)
   Recurrence (for n ≥ 4):
   a(n) = [ (12n-6)a(n-1) - (19n-35)a(n-2) + (13n-32)a(n-3) - (4n-14)a(n-4) ] / (2(n+1))
   a(0)=1, a(1)=0, a(2)=0, a(3)=1 */
#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamically allocated cache of computed values   */
static size_t cache_cap = 0;         /* current capacity of cache                       */
static idx_t  cache_len = 0;         /* number of entries currently stored in cache     */

seq_t a(idx_t n)
{
    /* If the requested value is already cached, return it immediately. */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache to fit all indices up to n. */
    size_t new_cap = (size_t)n + 1;
    if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed – terminate */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute all missing values up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else if (i == 1 || i == 2) {
            val = (seq_t)0;
        } else if (i == 3) {
            val = (seq_t)1;
        } else {
            seq_t si = (seq_t)i;                        /* cast i to seq_t once */
            seq_t c1 = 12*si - 6;                       /* coeff for a[i-1]   */
            seq_t c2 = 13*si - 32;                      /* coeff for a[i-3]   */
            seq_t c3 = 19*si - 35;                      /* coeff for a[i-2]   */
            seq_t c4 = 4*si - 14;                       /* coeff for a[i-4]   */

            seq_t term1 = c1 * cache[i-1];
            seq_t term2 = c2 * cache[i-3];
            seq_t numer  = term1 + term2;

            seq_t part1 = c3 * cache[i-2];
            seq_t part2 = c4 * cache[i-4];
            seq_t sigma = numer - part1 - part2;        /* exact numerator    */

            seq_t denom = (seq_t)(2 * (i + 1));          /* denominator        */
            val = sigma / denom;                        /* exact division     */
        }
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
