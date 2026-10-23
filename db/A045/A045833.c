/*
 * A045833  Expansion of eta(q^9)^3 / eta(q^3) in powers of q.
 * https://oeis.org/A045833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 0 if 3 | n or any prime factor p≡2 mod 3 occurs with odd exponent.
 * Otherwise a(n) = ∏_{p≡1 (mod 3)} (e+1), where e is the exponent of p in n.
 * This implements the multiplicative definition given in the OEIS entry.
 */
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Fast rejection: any factor of 3 ⇒ a(n)=0 */
    if ((n % 3) == 0) return 0;

    uint64_t temp = (uint64_t)n;
    seq_t result = 1;

    /* Handle factor 2 separately (2 ≡ 2 mod 3) */
    uint64_t e = 0;
    while ((temp & 1ULL) == 0) { temp >>= 1; ++e; }
    if (e > 0) {
        /* For p ≡ 2 mod 3, odd exponent kills the term */
        if (e & 1U) return 0;
        /* even exponent gives factor 1, nothing to multiply */
    }

    /* Trial division by odd p ≥ 3, skip p=3 as n is not divisible by 3 */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (p == 3) continue;          /* already excluded */
        if (temp % p != 0) continue;

        e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }

        if (p % 3 == 1) {
            /* Multiplicative factor (e+1) for primes ≡ 1 (mod 3) */
            result *= (seq_t)(e + 1);
        } else { /* p % 3 == 2 */
            /* Odd exponent kills the product */
            if (e & 1U) return 0;
            /* even exponent contributes factor 1 */
        }
    }

    /* Remaining factor (if any) is a prime > sqrt(original n) */
    if (temp > 1) {
        if (temp % 3 == 1) {
            /* exponent is 1 → factor (1+1)=2 */
            result *= (seq_t)2;
        } else { /* temp % 3 == 2 */
            /* exponent 1 is odd → a(n)=0 */
            return 0;
        }
    }

    return result;
}
