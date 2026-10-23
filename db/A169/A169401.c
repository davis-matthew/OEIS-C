/*
 * A169401  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1.
   For n≥1 define b(n)=1 if n==0, 2 if 1≤n≤31, 1 if n==32, else 0.
   Then
   a(n)=2·Σ_{k=1}^{31} a(n−k) − 3·a(n−32) + b(n)      (a(k)=0 for k<0). */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached sequence values   */
static size_t cache_len = 0;      /* number of values stored */
static size_t cache_cap = 0;      /* allocated capacity      */

/* Ensure that cache contains a(0)…a(n).  */
static void ensure_terms(idx_t n)
{
    if ((size_t)n < cache_len) return;         /* already computed */

    size_t target = (size_t)n + 1;
    if (target > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, target * sizeof(seq_t));
        if (!tmp) abort();                     /* out of memory   */
        cache     = tmp;
        cache_cap = target;
    }

    for (size_t i = cache_len; i < target; ++i) {
        seq_t sumPrev = 0;

        /* Terms a(i-1)…a(i-31) */
        for (size_t k = 1; k <= 31; ++k) {
            if (i >= k) sumPrev += 2 * cache[i - k];
        }

        /* Term a(i-32) with coefficient -3 */
        if (i >= 32) sumPrev -= 3 * cache[i - 32];

        /* b(i) */
        seq_t b;
        if (i == 0)          b = 1;
        else if (i <= 31)    b = 2;
        else if (i == 32)    b = 1;
        else                 b = 0;

        cache[i] = sumPrev + b;
    }

    cache_len = target;
}

seq_t a(idx_t n)
{
    ensure_terms(n);
    return cache[(size_t)n];
}
