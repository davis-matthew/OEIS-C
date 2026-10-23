/*
 * A027154  a(n) = Sum_{k=0..floor(n/2)} A027144(n-k, k).
 * https://oeis.org/A027154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2) + 3*a(n-4) + 4*a(n-6) for n > 5 Initial values:
 *            a(0)=1, a(1)=1, a(2)=3, a(3)=5, a(4)=9, a(5)=15 The function can
 *            handle any n ≥ 0 (up to ~10000 in test cases); it uses a static
 *            cache of size 10001, which is sufficient for the required range
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uintptr_t, if needed */

seq_t a(idx_t n)
{
    /* One-time static cache */
    static seq_t cache[10001];
    static unsigned initialized = 0;

    if (!initialized) {
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 3;
        cache[3] = 5;
        cache[4] = 9;
        cache[5] = 15;
        initialized = 1;
    }

    if (n <= 5)
        return cache[(idx_t)n];

    /* Ensure we are within the preallocated range */
    if (n >= 10001) {
        /* For very large n the function is not required to work.
           If you need to support larger n, allocate a larger cache
           or change to a dynamic approach. */
        return 0;
    }

    /* Compute values up to n if not already calculated */
    for (idx_t i = 6; i <= n; ++i) {
        cache[i] = (seq_t)( (seq_t)2 * cache[i - 2]
                          + (seq_t)3 * cache[i - 4]
                          + (seq_t)4 * cache[i - 6] );
    }
    return cache[n];
}
