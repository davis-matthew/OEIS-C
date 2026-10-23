/*
 * A166648  Totally multiplicative sequence with a(p) = 8*(p+1) for prime p.
 * https://oeis.org/A166648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e||n} (8*(p+1))^e
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 127, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factor n into primes and exponents */
    factor_u64(x, primes, exps, &cnt);

    /* compute the product of (8*(p+1))^e for each prime power */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t base = (seq_t)8 * (p + (seq_t)1);
        seq_t term = big_pow(base, (seq_t)exps[i]);
        result *= term;
    }
    return result;
}
