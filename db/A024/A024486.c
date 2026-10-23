/*
 * A024486  a(n) = (1/(2n+1))*Multinomial(3n; n,n,n).
 * https://oeis.org/A024486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n)! / ((2*n+1)*(n!)^3)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Euclidean GCD for 64‑bit integers */
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
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    seq_t res = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        /* Compute numerator and denominator in 64‑bit arithmetic
           to avoid overflow on intermediate values. */
        uint64_t ii   = (uint64_t)i;
        uint64_t numer = 3ULL
                       * (2ULL * ii - 1ULL)
                       * (3ULL * ii - 2ULL)
                       * (3ULL * ii - 1ULL);
        uint64_t denom = ii * ii * (2ULL * ii + 1ULL);

        /* Cancel common factors between numer and denom */
        uint64_t g = gcd64(numer, denom);
        numer /= g;
        denom /= g;

        /* Cancel remaining denom factors with the running result */
        g = gcd64((uint64_t)res, denom);
        res /= (seq_t)g;
        denom /= g;

        /* The residual denom must now be 1;
           multiply the remaining numerator using 128‑bit to avoid overflow. */
        unsigned __int128 prod = (unsigned __int128)res * (unsigned __int128)numer;
        res = (seq_t)(prod / denom);
    }

    return res;
}
