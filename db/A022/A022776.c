/*
 * A022776  Place where n-th 1 occurs in A023115.
 * https://oeis.org/A022776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + Σ_{k=1}^{n-1} ⌊k/√2⌋
 * Range:     u8 n <= 26, u16 n <= 430, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1, but guard against 0 */
        return 0;

    /* 80 bits of precision are sufficient for all k ≤ 10 000.
       The product k * R fits in 128 bits. */
    const unsigned int L = 80U;
    static unsigned __int128 SCALE = ((unsigned __int128)1 << L);
    static unsigned __int128 S = 0;   /* ≈ √2 * 2^L  */
    static unsigned __int128 R = 0;   /* ≈ √2/2 * 2^L */
    static int initialized = 0;

    if (!initialized) {
        long double sqrt2 = sqrtl((long double)2.0L);
        S = (unsigned __int128)(sqrt2 * (long double)SCALE);
        R = S >> 1;      /* floor(√2 * 2^(L-1)) */
        initialized = 1;
    }

    unsigned __int128 sum = 0;
    for (idx_t k = 1; k < n; ++k) {
        unsigned __int128 term = ((unsigned __int128)k * R) >> L;
        sum += term;
    }

    seq_t result = (seq_t)n + (seq_t)sum;
    return result;
}
