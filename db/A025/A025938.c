/*
 * A025938  Expansion of 1/((1-2*x)*(1-3*x)*(1-6*x)*(1-8*x)).
 * https://oeis.org/A025938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*8^(n+3)-5*6^(n+3)+8*3^(n+3)-5*2^(n+3))/120
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   ensure_cache, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the recurrence: a(n) = 3^(n+1)-2^(n+1)+14*a(n-1)-48*a(n-2) */

#include <stddef.h>
#include "../utils/ensure_cache.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last_computed = (idx_t)-1; /* no values computed yet */
    static seq_t pow2_curr = 0, pow3_curr = 0; /* current powers for index last_computed */

    /* Ensure enough space for n+1 elements */
    if (!cache || (size_t)(n + 1) > cap)
        ensure_cache((idx_t)(n + 1), &cache, &cap);

    /* Initialise first term if not yet done */
    if (last_computed == (idx_t)-1) {
        cache[0] = (seq_t)1;
        last_computed = 0;
    }

    if (n == 0)
        return cache[0];

    /* Initialise second term and power helpers if not yet done */
    if (last_computed < 1) {
        cache[1] = (seq_t)19;
        /* powers for index 1: 2^(1+1) = 4, 3^(1+1) = 9 */
        pow2_curr = pow2((idx_t)2);
        pow3_curr = pow3((idx_t)2);
        last_computed = 1;
    }

    /* Compute remaining terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t pow2_next = pow2_curr * (seq_t)2;
        seq_t pow3_next = pow3_curr * (seq_t)3;

        seq_t term = pow3_next - pow2_next
                   + (seq_t)14 * cache[i - 1]
                   - (seq_t)48 * cache[i - 2];

        cache[i] = term;

        pow2_curr = pow2_next;
        pow3_curr = pow3_next;
    }

    last_computed = n;
    return cache[n];
}
