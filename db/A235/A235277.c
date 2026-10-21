/*
 * A235277  Number of (n+1) X (7+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 5
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 15*a(n-2) - 15*a(n-3) - 80*a(n-4) + 80*a(n-5) +
 *            180*a(n-6) - 180*a(n-7) - 144*a(n-8) + 144*a(n-9) Initial
 *            values: a(1) = 6208 a(2) = 8012 a(3) = 10088 a(4) = 14672 a(5) =
 *            20440 a(6) = 33464 a(7) = 51800 a(8) = 93992 a(9) = 160888
 * Range:     u8 none, u16 n <= 7, u32 n <= 22, u64 n <= 46, u128 n <= 96,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>           /* for size_t */
#include "../utils/ensure_capacity.h"    /* ensure_capacity function */

/* Returns the n-th term of sequence A235277.  idx_t is a 64‑bit unsigned integer.
   The function memoizes computed terms across calls and works for any n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;     /* Undefined for 0, return 0 */

    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* number of allocated indices (0..cap-1) */

    /* first call – initialise cache and base values */
    if (cap == 0) {
        ensure_capacity(10, &cap, (void **)&cache, sizeof(seq_t));
        cache[1] = (seq_t)6208;
        cache[2] = (seq_t)8012;
        cache[3] = (seq_t)10088;
        cache[4] = (seq_t)14672;
        cache[5] = (seq_t)20440;
        cache[6] = (seq_t)33464;
        cache[7] = (seq_t)51800;
        cache[8] = (seq_t)93992;
        cache[9] = (seq_t)160888;
    }

    /* enlarge cache if necessary */
    if ((size_t)n >= cap) {
        ensure_capacity(n + 1, &cap, (void **)&cache, sizeof(seq_t));
    }

    /* term already computed? */
    if (cache[n] != (seq_t)0)
        return cache[n];

    /* compute all terms up to n */
    for (idx_t i = 10; i <= n; ++i) {
        seq_t v =  cache[i-1]
               + 15 * cache[i-2]
               - 15 * cache[i-3]
               - 80 * cache[i-4]
               + 80 * cache[i-5]
               + 180 * cache[i-6]
               - 180 * cache[i-7]
               - 144 * cache[i-8]
               + 144 * cache[i-9];
        cache[i] = v;
    }

    return cache[n];
}
