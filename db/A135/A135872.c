/*
 * A135872  Multiply the positive integers which are coprime to n in order
 *          (starting at 1). a(n) is the smallest such partial product that is
 *          >= n.
 * https://oeis.org/A135872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   _gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A135872:
 * a(n) = the smallest partial product of the positive integers
 * that are coprime to n (in increasing order, starting at 1)
 * which is ≥ n.  a(1)=1, a(2)=3, a(3)=8, … */
#include <stdlib.h>
#include "../utils/_gcd_idx.h"

/* Euclidean algorithm for gcd on integer type idx_t */

/* Cache of previously computed a(n) values */
static seq_t *a_cache = NULL;
static idx_t a_cache_size = 0;

/* Allocate or grow the cache up to at least n */
static void _ensure_cache(idx_t n)
{
    if (n <= a_cache_size)
        return;

    /* select a new size with a small slack to avoid reallocating too often */
    size_t new_size = (size_t)n + 10;
    seq_t *tmp = a_cache;
    if (tmp == NULL) {
        tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
    } else {
        tmp = (seq_t *)realloc(tmp, new_size * sizeof(seq_t));
    }
    if (tmp == NULL)
        exit(EXIT_FAILURE);          /* memory allocation failed */

    a_cache = tmp;

    /* compute values that were missing */
    idx_t start = (a_cache_size ? a_cache_size + 1 : 1);
    for (idx_t k = start; k <= n; ++k) {
        seq_t prod = (seq_t)1;
        if (k == 1) {
            /* for n = 1 the partial product is trivially 1 */
            prod = (seq_t)1;
        } else {
            for (idx_t i = 1; /* empty */; ++i) {
                if (_gcd_idx(i, k) == 1) {
                    prod = prod * (seq_t)i;
                    if (prod >= (seq_t)k)
                        break;
                }
            }
        }
        a_cache[k - 1] = prod;
    }

    a_cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* undefined for n < 1, but guard */
    _ensure_cache(n);
    return a_cache[n - 1];
}
