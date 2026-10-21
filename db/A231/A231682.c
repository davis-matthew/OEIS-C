/*
 * A231682  a(n) = Sum_{i=0..n} digsum_8(i)^3, where digsum_8(i) = A053829(i).
 * https://oeis.org/A231682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} (digsum_8(i))^3, where digsum_8(i) is the
 *            sum of the digits of i in base 8
 * Range:     u8 n <= 5, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   digsum_8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digsum_8.h"

/* memoized sequence storage */
static seq_t *cached      = NULL;  /* array of a(i) */
static idx_t cached_size  = 0;     /* allocated size in elements   */
static idx_t max_index    = 0;     /* highest i computed so far */

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0 && cached_size == 0) {
        /* initialize with a(0) = 0 */
        if (!(cached = (seq_t *)malloc(sizeof(seq_t)))) return (seq_t)0;
        cached[0] = (seq_t)0;
        cached_size = 1;
        max_index = 0;
        return cached[0];
    }

    if (n >= cached_size) {
        /* grow the cache to size n+1 */
        idx_t new_size = n + 1;
        seq_t *newp = (seq_t *)realloc(cached, new_size * sizeof(seq_t));
        if (!newp) return (seq_t)0; /* out of memory (unlikely) */
        cached = newp;
        cached_size = new_size;
    }

    /* compute missing values up to n */
    for (idx_t i = max_index + 1; i <= n; ++i) {
        seq_t d   = digsum_8(i);
        seq_t d2  = d * d;
        seq_t d3  = d2 * d;          /* cube */
        if (i == 0) {
            cached[0] = 0;
        } else {
            cached[i] = cached[i - 1] + d3;
        }
    }
    max_index = n;
    return cached[n];
}
