/*
 * A135248  a(n) = 4*a(n-1) - 4*a(n-2) + 2*a(n-4), with a(0)=a(1)=a(2)=0, and
 *          a(3)=1.
 * https://oeis.org/A135248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + 2*a(n-4) a(0) = 0, a(1) = 0, a(2) =
 *            0, a(3) = 1
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 49, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Ensure cache contains values up to index n */
static void ensure_upto(idx_t n)
{
    /* If we already have the required range, nothing to do */
    if (cache_len > 0 && cache_len - 1 >= n) return;

    size_t new_len = (size_t)(n + 1);
    /* realloc may return NULL; in that rare event we abort (matching malloc behaviour) */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp && new_len > 0) abort();
    cache = tmp;

    /* Compute the new elements that were not previously cached */
    for (size_t i = cache_len; i < new_len; ++i) {
        if (i <= 3) {
            /* Base cases a(0)=a(1)=a(2)=0, a(3)=1 */
            cache[i] = (i == 3 ? (seq_t)1 : (seq_t)0);
        } else {
            seq_t t1 = 4 * cache[i - 1];
            seq_t t2 = 4 * cache[i - 2];
            seq_t t3 = 2 * cache[i - 4];
            cache[i] = t1 - t2 + t3;
        }
    }
    cache_len = new_len;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_upto(n);
    return cache[(size_t)n];
}
