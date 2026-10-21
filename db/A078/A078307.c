/*
 * A078307  a(n) = Sum_{d divides n} (-1)^(n/d+1)*d^3.
 * https://oeis.org/A078307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, changed
 * Formula:   a(n) = Σ_{d|n} (-1)^(n/d+1) * d³ (Offset 1: a(1)=1)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1604, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the sequence term for any n ≥ 1.
 * The implementation enumerates all divisors of n by iterating d from 1 to √n.
 * For each divisor d we add the signed contribution d³ whose sign is positive
 * when n/d is odd and negative when n/d is even.
 */
seq_t a(idx_t n) {
    if (n == 0) return 0;                  /* graceful handling of 0, though not used */

    uint64_t N = (uint64_t)n;
    long long sum = 0;                     /* signed accumulator, safe for n up to ~10^12 */

    /* Enumerate divisors using the property that d ≤ N/d when d is a divisor
     * smaller or equal to the square root of N.  This avoids overflow in d*d
     * and works for all 64‑bit values. */
    for (uint64_t d = 1; d <= N / d; ++d) {
        if (N % d == 0) {
            uint64_t d1 = d;
            uint64_t k1 = N / d1;
            int sign1 = (k1 & 1) ? 1 : -1;
            unsigned __int128 cube1 = (unsigned __int128)d1 * d1 * d1;
            sum += sign1 * (long long)cube1;

            uint64_t d2 = N / d1;          /* paired divisor, different when not a square */
            if (d2 != d1) {
                uint64_t k2 = N / d2;      /* k2 == d1 */
                int sign2 = (k2 & 1) ? 1 : -1;
                unsigned __int128 cube2 = (unsigned __int128)d2 * d2 * d2;
                sum += sign2 * (long long)cube2;
            }
        }
    }

    return (seq_t)sum;
}
