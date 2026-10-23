/*
 * A166659  Totally multiplicative sequence with a(p) = 10p-1 for prime p.
 * https://oeis.org/A166659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime powers p^e of (10*p-1)^e (OEIS
 *            A166659)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            do {
                m /= p;
                ++e;
            } while (m % p == 0);
            seq_t base = (seq_t)(10 * (seq_t)p - (seq_t)1);
            seq_t term = big_pow(base, (seq_t)e);
            result = seq_mul(result, term);
        }
    }
    if (m > 1) {                /* leftover prime factor */
        seq_t base = (seq_t)(10 * (seq_t)m - (seq_t)1);
        result = seq_mul(result, base);  /* exponent e = 1 */
    }
    return result;
}
