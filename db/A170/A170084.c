/*
 * A170084  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 25*a(n-1)+25*a(n-2)+...+25*a(n-36)-325*a(n-37)+c(n) where
 *            c(0)=1, c(1..36)=2, c(37)=1, c(n)=0 for n>37 This recurrence
 *            follows from the rational generating function given in the OEIS
 *            entry
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a static cache so that subsequent calls are 
   O(1) after the first full computation. */
#include <stdlib.h>
#include <string.h> /* for memcpy */

static seq_t *cache = NULL;      /* static array of computed values    */
static size_t cache_cap = 0;     /* allocated size of cache           */
static idx_t  cache_len = (idx_t)-1; /* number of computed values       */

/* Compute a term of the recurrence for index n (n>=1) */
static inline seq_t next_term(idx_t n)
{
    seq_t term = 0;
    /* sum 25*a[n-j] for j=1..36 */
    idx_t up_to = (n < 36) ? n : 36;
    for (idx_t j = 1; j <= up_to; ++j)
        term += 25 * cache[n - j];
    /* subtract 325*a[n-37] if applicable */
    if (n >= 37)
        term -= 325 * cache[n - 37];
    /* add the constant from the numerator */
    if (n == 0)
        term += 1;
    else if (n <= 36)
        term += 2;
    else if (n == 37)
        term += 1;
    return term;
}

seq_t a(idx_t n)
{
    /* Initialisation: compute a(0)=1 */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;
        cache_len = 0;
    }
    /* Grow cache if necessary */
    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < required)
            new_cap *= 2;
        seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
        memcpy(tmp, cache, cache_cap * sizeof(seq_t));
        free(cache);
        cache = tmp;
        cache_cap = new_cap;
    }
    /* Compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i)
        cache[i] = next_term(i);
    cache_len = n;
    return cache[n];
}
