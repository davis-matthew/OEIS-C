/*
 * A074823  a(n) = 2^omega(n)*mu(n)^2.
 * https://oeis.org/A074823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = 2^omega(n) * mu(n)^2 This is the product of 2 for each
 *            distinct prime divisor of n, provided that n is square‑free;
 *            otherwise the value is zero
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 1) return 1;                /* ω(1)=0, μ^2(1)=1 ⇒ 2^0=1 */

    uint64_t remaining = (uint64_t)n;
    seq_t result = 1;

    /* Handle factor 2 */
    if ((remaining & 1) == 0) {
        uint8_t exp = 0;
        do {
            remaining >>= 1;
            exp++;
        } while ((remaining & 1) == 0);
        if (exp > 1) return 0;          /* square factor detected */
        result *= (seq_t)2;             /* one distinct prime */
    }

    /* Check odd factors */
    for (uint64_t p = 3; p * p <= remaining; p += 2) {
        if (remaining % p == 0) {
            uint8_t exp = 0;
            do {
                remaining /= p;
                exp++;
            } while (remaining % p == 0);
            if (exp > 1) return 0;      /* square factor detected */
            result *= (seq_t)2;          /* one distinct prime */
        }
    }

    /* Remaining prime > 1, if any */
    if (remaining > 1) {
        result *= (seq_t)2;
    }

    return result;
}
