/*
 * A016725  Number of integer solutions to x^2+y^2+z^2 = n^2, allowing zeros
 *          and distinguishing signs and order.
 * https://oeis.org/A016725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = 6 * b(n), where b(n) is multiplicative: b(2^e) = 1,
 *            b(p^e) = p^e if p ≡ 1 (mod 4), b(p^e) = p^e + 2*(p^e−1)/(p−1) if
 *            p ≡ 3 (mod 4); a(0) = 1
 * Range:     u8 n <= 20, u16 n <= 4346, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    /* Handle special case n = 0 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Factor n into primes/powers */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute multiplicative function b(n) */
    seq_t bvalue = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t p_pow = pow_uint(p, e);

        seq_t component;
        if (p == 2ULL) {
            /* b(2^e) = 1 */
            component = 1;
        } else if ((p & 3ULL) == 1ULL) { /* p ≡ 1 (mod 4) */
            component = p_pow;
        } else { /* p ≡ 3 (mod 4) */
            /* component = p^e + 2 * (p^e − 1) / (p − 1) */
            seq_t numer = p_pow - 1;
            seq_t denom = p - 1; /* exact division */
            seq_t add   = numer / denom;
            component = p_pow + 2 * add;
        }
        bvalue = bvalue * component;
    }

    /* a(n) = 6 * b(n) for n > 0 */
    return bvalue * 6;
}
