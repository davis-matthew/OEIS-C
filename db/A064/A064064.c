/*
 * A064064  n-th step is to add a(n) to each previous number a(k) (including
 *          itself, i.e., k <= n) to produce n+1 more terms of the sequence,
 *          starting with a(0)=1.
 * https://oeis.org/A064064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1;
   For n > 0 let i = ⌊(√(1+8(n-1))−1)/2⌋
   where i satisfies 1 + i(i+1)/2 ≤ n ≤ 1 + i(i+1)/2 + i.
   Let base = 1 + i(i+1)/2  and  j = n − base.
   Then a(n) = a(i) + a(j).
   This recurrence follows from the definition
   of A064064 as the numbers obtained by adding a(n)
   to each previous element in the growing list. */
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;
    /* initialise the cache to store at least element 0 */
    if (cache == NULL) {
        cache_size = 1;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
    }
    /* extend cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = cache_size;
        while (new_size <= n) new_size <<= 1;
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        for (idx_t i = cache_size; i < new_size; ++i)
            cache[i] = (seq_t)0;
        cache_size = new_size;
    }
    /* return memoised value if already computed */
    if (cache[n] != (seq_t)0) return cache[n];
    /* base case */
    if (n == 0) return cache[0] = (seq_t)1;

    /* compute the step number i */
    unsigned long long temp = 1ULL + 8ULL * (n - 1);
    unsigned long long s = (unsigned long long)floor(sqrt((long double)temp));
    /* adjust to ensure s is floor(sqrt(temp)) */
    while ((s + 1)*(s + 1) <= temp) ++s;
    while (s*s > temp) --s;
    idx_t i = (idx_t)((s - 1) >> 1);            /* i = floor((√temp - 1)/2) */

    idx_t base = 1 + i*(i + 1)/2;
    idx_t j = n - base;

    seq_t val = a(i) + a(j);
    cache[n] = val;
    return val;
}
