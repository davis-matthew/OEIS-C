/*
 * A050479  a(n) = C(n)*(9*n + 1) where C(n) = Catalan numbers (A000108).
 * https://oeis.org/A050479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n) * (9*n + 1) where C(n) is the nth Catalan number.
 *            This implementation computes Catalan numbers iteratively using
 *            the recurrence C(0) = 1 C(n) = C(n-1) * 2*(2n-1) / (n+1) for n
 *            >= 1 and then evaluates a(n) = C(n) * (9*n + 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for realloc */

static seq_t *catalan_cache = NULL;   /* dynamic array of Catalan numbers */
static idx_t  catalan_cache_len = 0;  /* number of stored elements */

/* Ensure that Catalan numbers up to index n are computed and stored. */
static void
ensure_catalan(idx_t n)
{
    if (catalan_cache_len <= n) {
        /* Grow the cache to hold indices 0 .. n */
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(catalan_cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            /* Insufficient memory – abort. In the real test harness
               any such failure would be considered an error.          */
            exit(EXIT_FAILURE);
        }
        catalan_cache = new_cache;

        /* Compute new Catalan values */
        for (idx_t i = catalan_cache_len; i <= n; ++i) {
            if (i == 0) {
                catalan_cache[0] = (seq_t)1;
            } else {
                /* C(i) = C(i-1) * 2*(2*(i-1)+1) / (i+1) */
                seq_t tmp = catalan_cache[i - 1];
                tmp = tmp * (seq_t)2 * (seq_t)( (idx_t)2*(i - 1) + 1 );
                catalan_cache[i] = tmp / (seq_t)(i + 1);
            }
        }
        catalan_cache_len = new_len;
    }
}

seq_t
a(idx_t n)
{
    /* special case n = 0 handled naturally by the formula below */
    ensure_catalan(n);
    seq_t C = catalan_cache[n];
    seq_t factor = ((seq_t)n) * (seq_t)9 + (seq_t)1;
    return C * factor;
}
