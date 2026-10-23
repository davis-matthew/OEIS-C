/*
 * A129390  Expansion of phi(x) * phi(-x^5) / (chi(-x^2) * chi(-x^10)) in
 *          powers of x where phi(), chi() are Ramanujan theta functions.
 * https://oeis.org/A129390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(2*n + 1) where b is multiplicative with * b(2^e) = 0, *
 *            b(5^e) = 1, * b(p^e) = e+1 if p ≡ 1,3,7,9 (mod 20), * b(p^e) =
 *            (1 + (-1)^e)/2 if p ≡ 11,13,17,19 (mod 20)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* m = 2*n + 1 (always odd */ 
    uint64_t m = (uint64_t)2 * (uint64_t)n + 1ULL;
    if (m == 1)                     /* b(1) = 1 */
        return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t t = m;
    for (uint64_t p = 3; p <= t / p; p += 2) {      /* odd primes only */
        if (t % p == 0) {
            unsigned int e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            seq_t factor;
            if (p == 5) {
                factor = (seq_t)1;
            } else {
                unsigned int mod20 = p % 20;
                if (mod20 == 1 || mod20 == 3 || mod20 == 7 || mod20 == 9) {
                    factor = (seq_t)(e + 1);
                } else { /* mod20 == 11,13,17,19 */
                    factor = (e % 2 == 0) ? (seq_t)1 : (seq_t)0;
                }
            }
            result *= factor;
            if (result == (seq_t)0)
                return result;          /* early exit if zero */
        }
    }
    if (t > 1) {                     /* remaining prime factor */
        unsigned int e = 1;
        seq_t factor;
        if (t == 5) {
            factor = (seq_t)1;
        } else {
            unsigned int mod20 = (unsigned int)(t % 20);
            if (mod20 == 1 || mod20 == 3 || mod20 == 7 || mod20 == 9) {
                factor = (seq_t)(e + 1);
            } else { /* mod20 == 11,13,17,19 */
                factor = (e % 2 == 0) ? (seq_t)1 : (seq_t)0;
            }
        }
        result *= factor;
    }
    return result;
}
