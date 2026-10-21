/*
 * A062124  Fourth column of A046741.
 * https://oeis.org/A062124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (6 + 19*n + 18*n^2 + 9*n^3)/2
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 983, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* Return the n-th term of OEIS A062124.
   Uses 128‑bit arithmetic when available to avoid overflow before division. */
seq_t a(idx_t n)
{
    /* Promote to at least 128 bits for intermediate calculation */
#if defined(__SIZEOF_INT128__)
    __int128 nn = (__int128)n;
    __int128 numer = 6
                    + (__int128)19 * nn
                    + (__int128)18 * nn * nn
                    + (__int128)9  * nn * nn * nn;
    numer /= 2;
    return (seq_t)numer;
#else
    /* Fallback: compute using recurrence with memoization (safe for small n). */
    static seq_t cache[10001];
    static int initialized = 0;
    if (!initialized) {
        cache[0] = 3;
        cache[1] = 26;
        cache[2] = 94;
        cache[3] = 234;
        int i;
        for (i = 4; i < 10001; ++i)
            cache[i] = 4 * cache[i-1] - 6 * cache[i-2] + 4 * cache[i-3] - cache[i-4];
        initialized = 1;
    }
    if (n < 10001)
        return cache[n];
    /* For larger n, fall back to the polynomial using 128-bit if available. */
    __int128 nn = (__int128)n;
    __int128 numer = 6
                    + (__int128)19 * nn
                    + (__int128)18 * nn * nn
                    + (__int128)9  * nn * nn * nn;
    numer /= 2;
    return (seq_t)numer;
#endif
}
