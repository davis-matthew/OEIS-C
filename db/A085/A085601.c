/*
 * A085601  a(n) = 2 * (4^n + 2^n) + 1.
 * https://oeis.org/A085601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A085601
   Formula: a(n) = 2*(4^n + 2^n) + 1
   Derived recurrence: a(0)=5, a(1)=13, a(2)=41,
                       a(n) = 7*a(n-1) - 14*a(n-2) + 8*a(n-3)  (n ≥ 3)
   This implementation uses the recurrence for arbitrarily many terms
   while avoiding large intermediate powers.  Cache is updated lazily
   so calls in any order are efficient.  Only integer arithmetic
   allowed by the requirements. */

#include <stdlib.h>             /* malloc, realloc */

static seq_t *cache = NULL;     /* dynamic cache of computed values   */
static idx_t cache_size = 0;    /* allocated size (number of slots)   */
static idx_t max_index = 0;     /* highest index already computed    */

static void ensure_cache(idx_t n)
{
    /* allocate or enlarge cache to hold indices up to n (inclusive) */
    if (cache == NULL) {
        cache_size = 3 < n+1 ? n+1 : 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL) { exit(EXIT_FAILURE); }
        /* initialise first three values */
        cache[0] = (seq_t)5;
        cache[1] = (seq_t)13;
        cache[2] = (seq_t)41;
        max_index = 2;
    } else if (n >= cache_size) {
        /* grow cache if needed */
        size_t new_size = cache_size * 2;
        if (new_size < (size_t)(n + 1)) new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (tmp == NULL) { exit(EXIT_FAILURE); }
        cache = tmp;
        cache_size = new_size;
    }
    /* compute missing values forward */
    for (idx_t i = max_index + 1; i <= n; ++i) {
        seq_t prod7  = (seq_t)7 * cache[i-1];
        seq_t prod8  = (seq_t)8 * cache[i-3];
        seq_t prod14 = (seq_t)14 * cache[i-2];
        cache[i] = prod7 + prod8 - prod14;
    }
    max_index = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)13;
    if (n == 2) return (seq_t)41;
    ensure_cache(n);
    return cache[n];
}
