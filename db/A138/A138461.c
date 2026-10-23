/*
 * A138461  Inverse binomial transform of A000957.
 * https://oeis.org/A138461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   2*n*a(n) = -(n+6)*a(n-1) + (13*n-33)*a(n-2) + 3*(7*n-18)*a(n-3)
 *            + 9*(n-3)*a(n-4), with a(0)=0, a(1)=1, a(2)=-2, a(3)=4, a(4)=-6
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 23, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Cached sequence values.  The array grows as needed. */
static seq_t *seq_cache = NULL;
static idx_t  seq_max_computed = 0;
static size_t seq_cap = 0;

/* Helper to allocate/resize the cache. */
static void seq_ensure_capacity(idx_t n)
{
    if (n < seq_cap) return;
    size_t new_cap = (seq_cap == 0) ? 64 : seq_cap;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *new_buf = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_buf) abort();         /* abort on allocation failure */
    seq_cache = new_buf;
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Allocate cache on first use */
    if (seq_cache == NULL) {
        seq_ensure_capacity(n);
        seq_cache[0] = 0;            /* a(0) = 0   */
        seq_max_computed = 0;
    }

    /* Compute missing values up to n */
    for (idx_t k = seq_max_computed + 1; k <= n; ++k) {
        seq_t val;
        if (k == 0)             val = 0;
        else if (k == 1)        val = 1;
        else if (k == 2)        val = -2;
        else if (k == 3)        val = 4;
        else if (k == 4)        val = -6;
        else {
            seq_t a1 = seq_cache[k - 1];
            seq_t a2 = seq_cache[k - 2];
            seq_t a3 = seq_cache[k - 3];
            seq_t a4 = seq_cache[k - 4];

            seq_t term1 = -( (seq_t)(k + 6) * a1 );
            seq_t term2 = ( (seq_t)(13 * (int)(k) - 33) * a2 );
            seq_t term3 = ( (seq_t)3 * (seq_t)(7 * (int)(k) - 18) * a3 );
            seq_t term4 = ( (seq_t)9 * (seq_t)(k - 3) * a4 );

            seq_t rhs = term1 + term2 + term3 + term4;
            seq_t denom = (seq_t)(2 * (int)(k));
            val = rhs / denom;             /* division is exact */
        }
        seq_cache[k] = val;
    }
    seq_max_computed = (seq_max_computed > n) ? seq_max_computed : n;
    return seq_cache[n];
}
