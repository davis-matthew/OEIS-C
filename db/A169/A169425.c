/*
 * A169425  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Such that the function satisfies the OEIS specification:
   a(0) = 1,
   for 1 <= n <= 31:   a(n) = 26 * Σ_{k=0}^{n-1} a(k) + 2,
   for n = 32:         a(32) = 26 * Σ_{k=1}^{31} a(32-k) - 351*a(0) + 1,
   for n > 32:         a(n) = 26 * Σ_{k=1}^{31} a(n-k) - 351*a(n-32) */
 
static seq_t *cache = NULL;    /* cache of already computed values */
static idx_t cache_len = 0;    /* number of elements stored (>= cache_len) */
 
/* Allocate or grow cache to hold at least new_len elements. */
static void grow_cache(idx_t new_len)
{
    if (new_len <= cache_len)
        return;
    /* realloc handles NULL pointer as malloc */
    seq_t *ptr = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
    cache = ptr;
    cache_len = new_len;
}
 
seq_t a(idx_t n)
{
    /* Ensure we can store at least up to n */
    grow_cache(n + 1);
 
    /* Compute values that are not yet known. */
    for (idx_t i = 0; i <= n; ++i)
    {
        if (i == 0)
        {
            cache[0] = (seq_t)1;
            continue;
        }
 
        /* Compute the sum of the previous up to 31 terms. */
        seq_t sum = (seq_t)0;
        idx_t j_max = (i < 31) ? i : 31;
        for (idx_t j = 1; j <= j_max; ++j)
            sum += cache[i - j];
 
        if (i <= 31)
        {
            /* a(i) = 26 * sum_{k=0}^{i-1} a(k) + 2 */
            cache[i] = (seq_t)26 * sum + (seq_t)2;
        }
        else if (i == 32)
        {
            /* a(32) = 26 * sum_{k=1}^{31} a(32-k) - 351*a(0) + 1 */
            cache[i] = (seq_t)26 * sum - (seq_t)351 * cache[0] + (seq_t)1;
        }
        else /* i > 32 */
        {
            /* a(i) = 26 * sum_{k=1}^{31} a(i-k) - 351 * a(i-32) */
            cache[i] = (seq_t)26 * sum - (seq_t)351 * cache[i - 32];
        }
    }
 
    return cache[n];
}
