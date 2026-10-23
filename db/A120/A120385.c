/*
 * A120385  If a(n-1) = 1 then largest value so far + 1, otherwise
 *          floor(a(n-1)/2); or table T(n,k) with T(n,0) = n, T(n,k+1) =
 *          floor(T(n,k)/2).
 * https://oeis.org/A120385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(1) = 1; a(n) = (a(n-1) == 1) ? (max_{i<n} a(i) + 1) :
 *            floor(a(n-1)/2)
 * Range:     u8 n <= 1793, u16 n <= 8183, u32 n <= 8183, u64 n <= 8183,
 *            u128 n <= 8183, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation buffer   */
    static seq_t *cache        = NULL;   /* array of a(0)..a(max_computed) */
    static size_t  alloc_size  = 0;      /* allocated array size          */
    static idx_t   max_computed = 0;     /* largest index with value      */
    static seq_t   largest       = 0;     /* largest value among a(i), i≤max_computed */

    if (n == 0)          /* base case never used (offset starts at 1) */
        return 0;

    /* ensure buffer is big enough for indices up to n   */
    if (!cache || (size_t)n + 1 > alloc_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp;
        if (!cache) {
            tmp = (seq_t *)calloc(new_size, sizeof(seq_t));
            if (!tmp) abort();
            cache      = tmp;
            alloc_size = new_size;
        } else {
            /* grow to at least new_size (power‑of‑two growth)   */
            size_t power = alloc_size;
            while (power < new_size) power <<= 1;
            tmp = (seq_t *)realloc(cache, power * sizeof(seq_t));
            if (!tmp) abort();
            cache      = tmp;
            alloc_size = power;
        }
    }

    /* initialise the first value if nothing has been computed yet */
    if (max_computed == 0) {
        cache[1]  = 1;
        largest   = 1;
        max_computed = 1;
    }

    /* compute forward only up to n, if necessary   */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t prev = cache[i-1];
        seq_t cur;
        if (prev == 1) {
            cur = largest + 1;
        } else {
            cur = prev / 2;   /* floor division is the default for integer types */
        }
        cache[i] = cur;
        if (cur > largest) largest = cur;
    }
    max_computed = n;

    return cache[n];
}
