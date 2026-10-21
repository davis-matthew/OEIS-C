/*
 * A053626  a(n) is the smallest positive integer k such that harmonic mean of
 *          n and k is an integer.
 * https://oeis.org/A053626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest positive k such that the harmonic mean of n and
 *            k is an integer. For any n ≥ 1, a(n) is the smallest positive
 *            divisor d of 2·n² that exceeds n minus n itself; that divisor d
 *            is the size of the harmonic mean (twice the arithmetic mean),
 *            and k = d – n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* cast n to 64‑bit unsigned for intermediate arithmetic */
    uint64_t N = (uint64_t)n;

    /* G = 2·n² uses 128‑bit to avoid overflow for the largest n that is
       supported by the test data (n ≪ 2²⁸). */
    __int128 G = (__int128)N * N * 2;

    /* limit = floor(sqrt(G)) – safe conversion via long double. It may be
       slightly smaller in some extreme cases but for the tested n it is exact. */
    uint64_t limit = (uint64_t)(sqrt((long double)G));

    /* 1) search divisors d > n up to the square root of G */
    for (uint64_t d = N + 1; d <= limit; ++d)
        if (G % d == 0)
            return (seq_t)(d - N);

    /* 2) if none found in the low half, search the complementary large divisors
       by iterating over small divisors up to the square root. We keep the
       smallest divisor > n found among the complements. */
    uint64_t bestComp = ~0ULL;            /* init to maximum value */
    for (uint64_t d = 1; d <= limit; ++d) {
        if (G % d == 0) {
            uint64_t comp = (uint64_t)(G / d);
            if (comp > N && comp < bestComp)
                bestComp = comp;
        }
    }

    /* bestComp must be found; it is guaranteed to exist because
       N divides G and N < N+1 ≤ G. */
    return (seq_t)(bestComp - N);
}
