/*
 * A054563  a(n) = n*(n^2 - 1)*(n + 2)*(n^2 + 4*n + 6)/72.
 * https://oeis.org/A054563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice, changed
 * Formula:   a(n) = n*(n^2-1)*(n+2)*(n^2+4*n+6)/72
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 81, u64 n <= 3314,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean algorithm for 64‑bit unsigned integers. */
static inline uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Offset 0, 1 give 0 by definition. */
    if (n <= 1)
        return 0;

    /* Work with 64‑bit values for the individual factors. */
    uint64_t n64 = (uint64_t)n;
    uint64_t f[5];

    f[0] = n64;               /* n          */
    f[1] = n64 - 1;           /* n-1        */
    f[2] = n64 + 1;           /* n+1        */
    f[3] = n64 + 2;           /* n+2        */

    /* n^2 + 4n + 6: use 128‑bit intermediate to avoid overflow. */
    unsigned __int128 tmp = (unsigned __int128)n64 * n64 +
                           4 * (unsigned __int128)n64 + 6;
    f[4] = (uint64_t)tmp;     /* fits in 64 bits for all n used in tests */

    uint64_t denom = 72;      /* 2^3 * 3^2 */

    /* Remove factors of the denominator from the 5 multiplicands. */
    for (int i = 0; i < 5 && denom > 1; ++i) {
        uint64_t g = gcd64(f[i], denom);
        if (g > 1) {
            f[i] /= g;
            denom /= g;
        }
    }
    /* In the unlikely event some factors remain, keep cancelling until all
       are removed. */
    while (denom > 1) {
        for (int i = 0; i < 5 && denom > 1; ++i) {
            uint64_t g = gcd64(f[i], denom);
            if (g > 1) {
                f[i] /= g;
                denom /= g;
            }
        }
    }

    /* Multiply the reduced factors. Use 128‑bit intermediate so the product
       never overflows when seq_t is 64‑bit; if seq_t is larger the cast
       will convert correctly. */
    unsigned __int128 prod = 1;
    for (int i = 0; i < 5; ++i)
        prod *= f[i];

    return (seq_t)prod;
}
