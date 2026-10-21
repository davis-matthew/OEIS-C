/*
 * A052978  Expansion of (1-2*x)/(1-4*x-2*x^2+4*x^3).
 * https://oeis.org/A052978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Recurrence: a(0)=1, a(1)=2, a(2)=10,
 *            a(n)=2*a(n-2)+4*a(n-1)-4*a(n-3) for n>=3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t computed = 0;   /* number of initialized entries */

    /* Ensure array large enough to hold up to index n */
    ensure_capacity((size_t)n + 1, &cap, (void **)&cache, sizeof(seq_t));

    /* Initialize base values as needed */
    if (computed == 0) {
        cache[0] = (seq_t)1;
        computed = 1;
    }
    if (computed == 1 && n >= 1) {
        cache[1] = (seq_t)2;
        computed = 2;
    }
    if (computed == 2 && n >= 2) {
        cache[2] = (seq_t)10;
        computed = 3;
    }

    /* Compute remaining terms if necessary */
    while (computed <= n) {
        idx_t i = computed;
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];

        /* a(i) = 4*a(i-1) - 4*a(i-3) + 2*a(i-2) */
        seq_t term = a1 * 4;
        term -= a3 * 4;
        term += a2 * 2;

        cache[i] = term;
        ++computed;
    }

    return cache[n];
}
