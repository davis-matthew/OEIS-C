/*
 * A053814  a(n) = n modulo (sum of proper divisors of n).
 * https://oeis.org/A053814
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n modulo (sum of proper divisors of n)
 * Range:     u8 n <= 48, u16 n <= 9704, u32 n <= 9704, u64 n <= 9704,
 *            u128 n <= 9704, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) for n >= 2 */
seq_t a(idx_t n) {
    // Compute σ(n), the sum of all divisors of n.
    seq_t sigma = 1;                // σ(n) starts at 1
    idx_t m = n;                    // Remaining part to factor

    // Test divisor 2 separately, then test odd numbers.
    if (m % 2 == 0) {
        seq_t power = 1;            // p^e (currently 2^0)
        idx_t e = 0;
        while (m % 2 == 0) {
            m /= 2;
            power *= (seq_t)2;
            ++e;
        }
        // factor = (2^(e+1)-1)/(2-1) = 2^(e+1)-1
        seq_t factor = power * (seq_t)2 - 1;
        sigma *= factor;
    }

    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            seq_t power = 1;        // p^e starting from p^0
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                power *= (seq_t)p;
                ++e;
            }
            // compute (p^(e+1)-1)/(p-1)
            seq_t powerNext = power * (seq_t)p;   // p^(e+1)
            seq_t numerator = powerNext - 1;
            seq_t denom = (seq_t)p - 1;
            seq_t factor = numerator / denom;
            sigma *= factor;
        }
    }

    // If there's a remaining prime factor > sqrt(original n).
    if (m > 1) {
        // For exponent e=1, the factor is (p^2-1)/(p-1) = p+1.
        sigma *= ((seq_t)m + 1);
    }

    // Proper divisor sum = σ(n) - n
    seq_t proper = sigma - (seq_t)n;

    // Return n modulo proper
    return (seq_t)n % proper;
}
