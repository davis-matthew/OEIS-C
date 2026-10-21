/*
 * A065928  (x,y) = (a(n),a(n+1)) are the solutions of (t(x)+t(y))/(1+xy) =
 *          t(2) = 3, where t(n) denotes the n-th triangular number t(n) =
 *          n(n+1)/2.
 * https://oeis.org/A065928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=2, a(1)=11, a(2)=63; a(n)=7*a(n-1)-7*a(n-2)+a(n-3) for n≥3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of stored elements  (= last index + 1) */

static void ensure_cache(idx_t up_to)
{
    if (up_to < cache_len)
        return;

    size_t old_len = cache_len;
    size_t new_len = up_to + 1;

    seq_t *tmp;
#ifdef __cplusplus
    /* C++: use new[] which calls constructors if necessary */
    tmp = new seq_t[new_len];
#else
    /* C: use malloc which is fine for trivial types */
    tmp = (seq_t *)malloc(new_len * sizeof(seq_t));
#endif
    if (!tmp)
        abort();                       /* termination on allocation failure */

    /* copy previously computed values */
    for (size_t i = 0; i < old_len; ++i)
        tmp[i] = cache[i];

    /* initialise missing early terms (a[0..2]) if they were not already set */
    if (old_len == 0) {
        tmp[0] = (seq_t)2;
        tmp[1] = (seq_t)11;
        if (new_len > 2)
            tmp[2] = (seq_t)63;
    } else if (old_len == 1) {
        if (new_len > 1)
            tmp[1] = (seq_t)11;
        if (new_len > 2)
            tmp[2] = (seq_t)63;
    } else if (old_len == 2) {
        if (new_len > 2)
            tmp[2] = (seq_t)63;
    }

    /* free old storage */
#ifdef __cplusplus
    delete[] cache;
#else
    free(cache);
#endif
    cache = tmp;
    cache_len = new_len;

    /* compute the recurrence for new indices, starting at 3 if necessary */
    idx_t start = old_len;
    if (start < (idx_t)3)
        start = 3;
    for (idx_t i = start; i <= up_to; ++i) {
        cache[i] = ((seq_t)7 * cache[i-1]) - ((seq_t)7 * cache[i-2]) + cache[i-3];
    }
}

seq_t a(idx_t n)
{
    if (n < 3) {
        /* handle the first three values without recursion */
        switch ((uint64_t)n) {
            case 0: return (seq_t)2;
            case 1: return (seq_t)11;
            case 2: return (seq_t)63;
            default: /* never reached */ break;
        }
    }
    ensure_cache(n);
    return cache[n];
}
