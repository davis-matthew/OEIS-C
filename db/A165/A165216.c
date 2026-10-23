/*
 * A165216  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7·(a(n-1)+a(n-2)+…+a(n-8)) – 28·a(n-9) (n ≥ 10) with
 *            initial values a(0)=1, a(1)=9, a(2)=72, a(3)=576, a(4)=4608,
 *            a(5)=36864, a(6)=294912, a(7)=2359296, a(8)=18874368,
 *            a(9)=150994908. The function uses a static cache so that any
 *            order of calls is O(1) after the first traversal. Required he...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* size_t */
#include <stdint.h>          /* uint64_t */

#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* static cache and its current length */
    static seq_t *cache   = NULL;
    static idx_t  cached  = 0UL;   /* number of elements stored */

    /* if the term is already cached, return it */
    if (n < cached)
        return cache[n];

    /* desired length after this call */
    idx_t new_len = n + 1;

    /* allocate new array (size_t is the parameter type of seq_alloc_array) */
    seq_t *new_cache = seq_alloc_array((size_t)new_len);
    if (!new_cache)                /* allocation failed – return zero */
        return (seq_t)0;

    /* copy old data if any */
    for (idx_t i = 0; i < cached; ++i)
        new_cache[i] = cache[i];

    /* initial values for a(0)..a(9) */
    static const seq_t init[10] = {
        (seq_t)1ULL,
        (seq_t)9ULL,
        (seq_t)72ULL,
        (seq_t)576ULL,
        (seq_t)4608ULL,
        (seq_t)36864ULL,
        (seq_t)294912ULL,
        (seq_t)2359296ULL,
        (seq_t)18874368ULL,
        (seq_t)150994908ULL
    };

    /* compute all new terms up to n */
    for (idx_t i = cached; i < new_len; ++i) {
        if (i < 10) {                 /* use the table directly   */
            new_cache[i] = init[i];
            continue;
        }
        seq_t sum = 0;
        /* 7 * (a(i-1)+...+a(i-8)) */
        for (idx_t k = 1; k <= 8; ++k)
            sum += new_cache[i - k] * (seq_t)7;
        /* minus 28 * a(i-9) */
        sum -= new_cache[i - 9] * (seq_t)28;
        new_cache[i] = sum;
    }

    /* replace old cache, freeing it */
    if (cache)
        seq_free_array(cache);

    cache = new_cache;
    cached = new_len;

    return cache[n];
}
