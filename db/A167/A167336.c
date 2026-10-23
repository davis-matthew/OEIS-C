/*
 * A167336  Totally multiplicative sequence with a(p) = 2*(4p+1) = 8p+2 for
 *          prime p.
 * https://oeis.org/A167336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A167336: Totally multiplicative sequence with a(p) = 8p+2. */
/* a(n) = product over p^e||n of (8p+2)^e */
#include <stdint.h>
#include "../utils/pow_uint.h"   /* seq_t pow_uint(uint64_t base, idx_t exp) */
#include "../utils/seq_mul.h"   /* seq_t seq_mul(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    if (n <= 1) /* a(1) = 1, and n is never 0 in the OEIS offset */
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t temp   = n;

    /* handle factor 2 */
    idx_t exp2 = 0;
    while ((temp & 1ULL) == 0) { /* temp % 2 == 0 */
        ++exp2;
        temp >>= 1;              /* temp /= 2 */
    }
    if (exp2) {
        uint64_t base = 8ULL * 2ULL + 2ULL;            /* 18 */
        seq_t term = pow_uint(base, exp2);
        result = seq_mul(result, term);
    }

    /* factor odd primes */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                ++e;
                temp /= p;
            }
            uint64_t base = 8ULL * p + 2ULL;
            seq_t term = pow_uint(base, e);
            result = seq_mul(result, term);
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        uint64_t base = 8ULL * temp + 2ULL;
        seq_t term = pow_uint(base, 1);   /* e = 1 */
        result = seq_mul(result, term);
    }

    return result;
}
