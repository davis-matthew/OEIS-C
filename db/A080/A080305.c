/*
 * A080305  Denominator of n^mu(n), where mu is the Moebius function
 *          (A008683).
 * https://oeis.org/A080305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = if μ(n) < 0 then n else 1 where μ is the Möbius function
 * Range:     u8 n <= 256, u16 n <= 8349, u32 n <= 8349, u64 n <= 8349,
 *            u128 n <= 8349, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int mobius(idx_t n) {
    if (n == 1) return 1;          /* μ(1) = 1 */
    idx_t temp = n;
    int prime_count = 0;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
                if (exp > 1) return 0;     /* squared prime factor detected */
            }
            ++prime_count;
        }
    }
    if (temp > 1) ++prime_count;      /* remaining prime factor */
    return (prime_count % 2 == 0) ? 1 : -1;
}

seq_t a(idx_t n) {
    int mu = mobius(n);
    return (mu < 0) ? (seq_t)n : (seq_t)1;
}
