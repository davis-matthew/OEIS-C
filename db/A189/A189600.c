/*
 * A189600  Number of permutations of 1..n with displacements restricted to
 *          {-7,-6,-5,-4,-3,-2,0,1}.
 * https://oeis.org/A189600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7)+a(n-8) for n >
 *            8, with initial terms: a(1)=1, a(2)=1, a(3)=2, a(4)=4, a(5)=7,
 *            a(6)=12, a(7)=21, a(8)=37
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 41, u64 n <= 81, u128 n <= 161,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n < 1 is outside the OEIS offset – return 0 */
    if (n < 1) return (seq_t)0;

    /* static storage for memoization */
#if defined(__cplusplus)
    static seq_t *cache = nullptr;
#else
    static seq_t *cache = NULL;
#endif
    static size_t cap = 0;          /* current capacity of cache */
    static size_t max_cached = 0;   /* largest index already computed */

    /* needed capacity (index n will be stored at cache[n]) */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        if (new_cap < need) new_cap = need;
#if defined(__cplusplus)
        seq_t *new_ptr = new seq_t[new_cap];
        if (cache) {
            for (size_t i = 0; i < cap; ++i) new_ptr[i] = cache[i];
            delete[] cache;
        }
        cache = new_ptr;
#else
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) return (seq_t)0;   /* allocation failure */
        cache = new_ptr;
#endif
        cap = new_cap;
    }

    /* initialize the first eight terms once */
    if (max_cached == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)4;
        cache[5] = (seq_t)7;
        cache[6] = (seq_t)12;
        cache[7] = (seq_t)21;
        cache[8] = (seq_t)37;
        max_cached = 8;
    }

    /* compute any missing terms up to n */
    if (max_cached < (size_t)n) {
        for (size_t i = max_cached + 1; i <= (size_t)n; ++i) {
            /* recurrence: a(i) = a(i-1)+a(i-3)+a(i-4)+a(i-5)+a(i-6)+a(i-7)+a(i-8) */
            seq_t sum = cache[i - 1];
            sum += cache[i - 3];
            sum += cache[i - 4];
            sum += cache[i - 5];
            sum += cache[i - 6];
            sum += cache[i - 7];
            sum += cache[i - 8];
            cache[i] = sum;
        }
        max_cached = (size_t)n;
    }

    return cache[(size_t)n];
}
