/*
 * A057434  a(n) = Sum_{k=1..n} phi(k)^2.
 * https://oeis.org/A057434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} φ(k)^2
 * Range:     u8 n <= 12, u16 n <= 76, u32 n <= 3108, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Keep a running prefix sum of φ(k)^2.  The array is 1‑based; index 0
       is a dummy initialized to 0.  The cache is grown lazily and reused
       across calls, so the function works in any call order. */
    static seq_t *sum_cache = NULL;
    static idx_t max_computed = 0;   /* largest n already computed */

    /* If requested value is already cached, return it. */
    if (n <= max_computed)
        return sum_cache[n];

    /* Grow the cache to hold indices up to n. */
    idx_t new_size = n + 1;                  /* indices 0..n inclusive */
    seq_t *tmp = (seq_t *)realloc(sum_cache, new_size * sizeof(seq_t));
    if (!tmp)                               /* allocation failed */
        return 0;                           /* caller must not expect a valid sum */

    sum_cache = tmp;

    /* Initialize the dummy entry if this is the first call. */
    if (max_computed == 0)
        sum_cache[0] = (seq_t)0;

    /* Compute missing values incrementally. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t phi_i = phi(i);               /* φ(i)  */
        seq_t sq    = phi_i * phi_i;        /* φ(i)^2  */
        sum_cache[i] = sum_cache[i - 1] + sq;
    }

    max_computed = n;
    return sum_cache[n];
}
