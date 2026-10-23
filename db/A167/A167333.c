/*
 * A167333  Totally multiplicative sequence with a(p) = 2*(5p-1) = 10p-2 for
 *          prime p.
 * https://oeis.org/A167333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e∥n} (10p – 2)^e (multiplicative, a(p) = 10p – 2)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    uint64_t temp = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(temp, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        seq_t base = (seq_t)10 * p - 2;
        seq_t pw = big_pow(base, (seq_t)e);
        result *= pw;
    }
    return result;
}
