/*
 * A099269  A sequence derived from a matrix using "0,1,2,3,4,5,6".
 * https://oeis.org/A099269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A099269:
 *  a(1)=1, a(2)=4, a(3)=32
 *  a(n)=7*a(n-1)+4*a(n-2)-18*a(n-3)   for n>=4   (Indexing starts at 1)
 *  Recurrence provides an O(1) computation per term after the first
 *  few values are stored.  The function memoises results so that
 *  repeated calls in any order remain fast.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised terms, 0‑based: cache[0] = a(1) */
static idx_t  cache_size = 0;    /* allocated size of cache array      */
static idx_t  last_computed = 0; /* index of last computed element     */

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* out of range; defined value irrelevant */

    idx_t idx = n - 1;                 /* convert to 0‑based index */

    /* ensure cache is large enough   */
    if (cache == NULL || idx + 1 > cache_size) {
        idx_t new_size = (cache_size == 0) ? 3 : cache_size;
        while (new_size <= idx)        /* grow until it can hold idx     */
            new_size *= 2;

        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        cache_size = new_size;
    }

    /* initialise the first three terms on first use   */
    if (last_computed == 0 && idx < 3) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)32;
        last_computed = 2;          /* up to index 2 (a(3)) ready   */
    }

    /* compute any missing terms up to idx   */
    if ((size_t)idx > last_computed) {
        for (idx_t i = last_computed + 1; i <= idx; ++i) {
            seq_t v1 = cache[i - 1];          /* a(i)      */
            seq_t v2 = cache[i - 2];          /* a(i-1)    */
            seq_t v3 = cache[i - 3];          /* a(i-2)    */

            seq_t term = (seq_t)7 * v1 + (seq_t)4 * v2 - (seq_t)18 * v3;
            cache[i] = term;
        }
        last_computed = idx;
    }

    return cache[idx];
}
