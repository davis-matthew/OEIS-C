/*
 * A032346  Essentially shifts 1 place right under inverse binomial transform.
 * https://oeis.org/A032346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, eigen
 * Formula:   a(n) = a(n-1) - sum_{k=0}^{n-1} (-1)^{n-k} * C(n,k) * a(k) for
 *            n>=2, with base values a(0)=1, a(1)=1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence A032346 is defined by the condition that its inverse
    binomial transform equals the original sequence shifted one place to
    the right. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated number of elements   */
    static idx_t  cache_len = 0;   /* next index to compute (size)   */

    /* (re)allocate cache if needed */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_cap) {
        seq_t *newptr = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!newptr) return 0;        /* out‑of‑memory, return 0 */
        cache = newptr;
        cache_cap = needed;
    }

    /* compute values up to index n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
            continue;
        }
        if (i == 1) {
            cache[1] = 1;
            continue;
        }

        /* sum_{k=0}^{i-1} (-1)^{i-k} * C(i,k) * a(k) */
        seq_t sum = 0;
        seq_t C = 1;          /* C(i,0) */
        for (idx_t k = 0; k < i; ++k) {
            seq_t term = C * cache[k];
            if (((i - k) & 1) == 0)   /* even exponent -> positive */
                sum += term;
            else                      /* odd exponent -> negative  */
                sum -= term;

            /* prepare C(i,k+1) for next iteration */
            if (k + 1 < i) {
                /* C(i,k+1) = C(i,k) * (i-k) / (k+1)   (exact division) */
                C = (C * (seq_t)(i - k)) / (seq_t)(k + 1);
            }
        }

        /* a(i) = a(i-1) - sum */
        cache[i] = cache[i - 1] - sum;
    }

    cache_len = n + 1;
    return cache[n];
}
