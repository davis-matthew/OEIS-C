/*
 * A102846  a(0)=1, a(1)=1, a(n) = a(n-1)*a(n-2) + 2.
 * https://oeis.org/A102846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)*a(n-2) + 2, with a(0) = 1, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Maximum n expected in the tests (≈10000). */
    #define MAX_N 10001

    /* Static cache for already computed terms. */
    static seq_t cache[MAX_N];
    static bool  known[MAX_N];
    static idx_t max_known = 1;   /* we pre‑initialise a(0) and a(1) */

    /* Initialize a(0) and a(1) only once. */
    if (!known[0]) {
        cache[0] = (seq_t)1;
        known[0] = true;
    }
    if (!known[1]) {
        cache[1] = (seq_t)1;
        known[1] = true;
    }

    /* If the requested term is already cached, return it. */
    if (n <= max_known)
        return cache[n];

    /* Compute missing terms up to n. */
    for (idx_t i = max_known + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] * cache[i - 2] + (seq_t)2;
        known[i] = true;
    }

    max_known = n;
    return cache[n];
}
