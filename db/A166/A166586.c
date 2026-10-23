/*
 * A166586  Totally multiplicative sequence with a(p) = p - 2 for prime p.
 * https://oeis.org/A166586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 0 when n is even and n > 1; otherwise a(n) = ∏_{p^e‖n,
 *            p>2} (p-2)^e with a(1)=1
 * Range:     u8 n <= 262, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq_t(seq_t base, uint64_t exp) {
    seq_t res = (seq_t)1;
    while (exp) {
        if (exp & 1) res *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;          /* a(1) = 1 */
    if ((n & 1ULL) == 0) return (seq_t)0; /* even n > 1 → 0 */

    uint64_t temp = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* trial division by odd primes */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            seq_t base = (seq_t)(p - 2);
            result *= pow_seq_t(base, exp);
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (temp > 1) {
        seq_t base = (seq_t)(temp - 2);
        result *= base;   /* exponent is 1 */
    }

    return result;
}
