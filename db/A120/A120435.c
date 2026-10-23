/*
 * A120435  Triangle read by rows: T(n,k) = lcm(1,2,3,4,...,n)/k (1 <= k <=
 *          n).
 * https://oeis.org/A120435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 78, u32 n <= 253, u64 n <= 1081,
 *            u128 n <= 3916, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): OEIS A120435 – triangle read by rows: T(n,k) = lcm(1,…,n)/k
   Linear index n is the position when rows are concatenated.
   The function computes the row r and position k for the given n,
   caches lcm(1..r) values, and returns lcm / k. */

#include <math.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* unused – offset starts at 1 */

    /* Find r: smallest r with r*(r+1)/2 >= n */
    idx_t r = (idx_t)((sqrt((double)(8.0 * n + 1.0)) - 1.0) / 2.0);
    while (r * (r + 1) / 2 < n)
        ++r;
    while (r > 0 && (r - 1) * r / 2 >= n)
        --r;

    /* Position within row */
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;          /* 1 <= k <= r */

    /* Cache for lcm(1..i) */
    static seq_t *lcm_cache = NULL;
    static idx_t cache_size = 0;

    if (cache_size <= r) {
        idx_t new_size = r + 1;
        seq_t *new_cache = (seq_t *)realloc(lcm_cache, new_size * sizeof(seq_t));
        if (!new_cache) /* Allocation failed; terminate gracefully */
            return (seq_t)0;
        if (cache_size == 0) {          /* init first entries */
            new_cache[0] = (seq_t)1;
            new_cache[1] = (seq_t)1;
            cache_size = 1;             /* will be increased below */
        }
        for (idx_t i = cache_size; i <= r; ++i) {
            seq_t prev_lcm = new_cache[i - 1];
            seq_t g = gcd(prev_lcm, (seq_t)i);
            new_cache[i] = (prev_lcm / g) * (seq_t)i;
        }
        lcm_cache = new_cache;
        cache_size = new_size;
    }

    seq_t l = lcm_cache[r];
    return l / (seq_t)k;
}
