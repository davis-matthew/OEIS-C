/*
 * A083884  a(n) = (3^(2*n) + 1) / 2.
 * https://oeis.org/A083884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A083884
 *  a(n) = (3^(2*n) + 1) / 2
 *  Recurrence: a(0) = 1,  a(n) = 9*a(n-1) - 4  for n >= 1
 *
 *  The function caches computed terms so that calls in any order
 *  are efficient.  It uses only integer arithmetic on the type
 *  'seq_t', which may be a small unsigned integer or an
 *  arbitrary‑precision big integer class.
 */

#include <stdlib.h>   /* for malloc, realloc, abort */

static seq_t *cache = NULL;   /* array of previously computed terms */
static idx_t  cache_len = 0;  /* number of terms stored, i.e. cache_len = highest index + 1 */

/* Return the n-th term of A083884 */
seq_t a(idx_t n)
{
    /* initialise cache for a(0) */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));   /* room for a(0) */
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* extend cache up to n */
    if (n >= cache_len) {
        idx_t new_len = n + 1;                     /* indices 0 .. n */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (idx_t i = cache_len; i <= n; ++i) {
            /* a(i) = 9*a(i-1) - 4 */
            seq_t val = cache[i-1];
            val = val * (seq_t)9;
            val = val - (seq_t)4;
            cache[i] = val;
        }
        cache_len = new_len;
    }

    return cache[n];
}
