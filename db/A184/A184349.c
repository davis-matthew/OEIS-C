/*
 * A184349  Number of strings of numbers x(i=1..3) in 0..n with sum i^4*x(i)
 *          equal to 81*n.
 * https://oeis.org/A184349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x1,x2,x3) ∈ {0,..,n}^3 | x1 + 16·x2 + 81·x3 = 81·n}
 * Range:     u8 n <= 142, u16 n <= 998, u32 n <= 998, u64 n <= 998,
 *            u128 n <= 998, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper: floor division of int64_t by positive int64_t divisor (b>0) */
static inline int64_t floor_div(int64_t a, int64_t b)
{
    int64_t d = a / b;
    int64_t r = a % b;
    if (r != 0 && a < 0) d -= 1;
    return d;
}

/* Helper: ceil division of int64_t by positive int64_t divisor (b>0) */
static inline int64_t ceil_div(int64_t a, int64_t b)
{
    int64_t d = a / b;
    int64_t r = a % b;
    if (r != 0 && a > 0) d += 1;
    return d;
}

/* Cache for computed values */
static seq_t *cache = NULL;
static size_t cache_size = 0;

/* External function implementing the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence offset starts at 1 */
    /* extend cache if necessary */
    if (cache_size <= n) {
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;        /* allocation failure, return 0 as safe value */
        cache = tmp;
        for (size_t i = cache_size; i < new_size; ++i) cache[i] = 0;
        cache_size = new_size;
    }
    if (cache[n] != 0) return cache[n];  /* already computed */

    /* compute value for n */
    seq_t count = 0;
    for (idx_t x3 = 0; x3 <= n; ++x3) {
        idx_t m = n - x3;
        int64_t term1 = (int64_t)81 * (int64_t)m;
        int64_t lower = ceil_div(term1 - (int64_t)n, 16);
        int64_t upper = floor_div(term1, 16);
        if (lower < 0) lower = 0;
        if (upper > (int64_t)n) upper = n;
        if (upper >= lower) count += (seq_t)(upper - lower + 1);
    }
    cache[n] = count;
    return count;
}
