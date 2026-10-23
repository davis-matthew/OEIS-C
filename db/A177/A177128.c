/*
 * A177128  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=7, k=0 and l=1.
 * https://oeis.org/A177128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1, a(1) = 7
   a(n+1) = Σ_{p=0}^{n} a(p)·a(n-p) + 1   for n ≥ 1   (k = 0, l = 1)
   This routine memoises the sequence in a static dynamic array. */

#include <stdlib.h>

/* Helper that expands the cache to hold at least (n+1) elements.
   The old contents are preserved. */
static void grow_cache(idx_t n, seq_t **cache, idx_t *len_ptr)
{
    idx_t old_len = *len_ptr;
    if (old_len > n) return;               /* already big enough */

    idx_t new_len = n + 1;
#ifdef __cplusplus
    /* C++: use new[]/delete[] and copy the old part */
    seq_t *new_cache = new seq_t[new_len];
    for (idx_t i = 0; i < old_len; ++i) new_cache[i] = (*cache)[i];
    delete [] *cache;
    *cache = new_cache;
#else
    /* C: use realloc, which copies the existing block */
    seq_t *new_cache = (seq_t *)realloc(*cache, new_len * sizeof(seq_t));
    *cache = new_cache;
#endif
    *len_ptr = new_len;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of elements stored */

    /* Ensure the cache can hold a[0..n] */
    grow_cache(n, &cache, &cache_len);

    /* Compute any missing terms */
    for (idx_t i = 0; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)7;
        } else {
            /* a(i) = Σ_{p=0}^{i-1} a(p)*a(i-1-p) + 1   (k=0,l=1) */
            seq_t sum = (seq_t)1;
            for (idx_t p = 0; p < i; ++p)
                sum += cache[p] * cache[i-1-p];
            cache[i] = sum;
        }
    }

    return cache[n];
}
