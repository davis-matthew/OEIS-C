/*
 * A151849  a(0)=1; a(1)=1; a(2)=2; for n>=3 if n=2^i + j with 0<=j<2^i then
 *          a(n)=a(j) + a(j + 1) except we add 1 if j=2^i-1.
 * https://oeis.org/A151849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(1)=1; a(2)=2; * for n>=3: let i=floor(log2(n)), j =
 *            n-2^i. * a(n) = a(j) + a(j+1) (+1 if j=2^i-1)
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* for size_t */
#ifdef __cplusplus
# include <cstddef>          /* for size_t in C++ */
# include <new>             /* for ::operator new/delete[] */
#else
# include <stdlib.h>        /* for malloc/realloc/free */
#endif

static seq_t *cache = NULL;          /* dynamic storage for a(n) values   */
static idx_t  cache_len = 0;         /* size of the allocated array       */
static idx_t  computed_len = 0;      /* first index for which the value
                                       has been computed (exclusive)       */

static void ensure_cache(idx_t n)
{
    idx_t new_len = n + 1;
    if (new_len <= cache_len)
        return;                      /* already large enough */

#if defined(__cplusplus)
    /* Reallocate using new/delete[] to respect constructors */
    seq_t *tmp = new seq_t[new_len];
    if (cache) {
        /* copy existing values */
        for (idx_t i = 0; i < cache_len; ++i)
            tmp[i] = cache[i];
        delete[] cache;
    }
    cache = tmp;
#else
    /* C-style realloc */
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    cache = tmp;
#endif
    cache_len = new_len;
}

static idx_t floor_log2(idx_t n)
{
    idx_t i = 0;
    while (n > 1) {
        n >>= 1;
        ++i;
    }
    return i;
}

seq_t a(idx_t n)
{
    if (n < 3) {
        /* Base cases: a(0)=1, a(1)=1, a(2)=2 */
        if (n == 0) return (seq_t)1;
        if (n == 1) return (seq_t)1;
        return (seq_t)2;
    }

    /* Ensure storage for at least n+1 elements */
    ensure_cache(n);

    /* Initialise base elements once */
    if (computed_len == 0) {
        cache[0] = (seq_t)1;
        if (cache_len > 1) cache[1] = (seq_t)1;
        if (cache_len > 2) cache[2] = (seq_t)2;
        computed_len = 3;
    }

    /* Compute missing values */
    for (idx_t idx = computed_len; idx <= n; ++idx) {
        idx_t i   = floor_log2(idx);
        idx_t pow = ((idx_t)1) << i;
        idx_t j   = idx - pow;

        seq_t sum = cache[j] + cache[j + 1];
        if (j == pow - 1)
            sum += (seq_t)1;
        cache[idx] = sum;
    }
    if (n + 1 > computed_len)
        computed_len = n + 1;

    return cache[n];
}
