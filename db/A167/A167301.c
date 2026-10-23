/*
 * A167301  Totally multiplicative sequence with a(p) = 9*(p-2) for prime p.
 * https://oeis.org/A167301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(1) = 1; a(2k) = 0; a(p^e) = (9*(p-2))^e for prime p; for
 *            composite n, // a(n) = ∏_{p|n} (9*(p-2))^{e(p)}
 * Range:     u8 n <= 20, u16 n <= 314, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    /* a(1) = 1 */
    if (n == 1) return (seq_t)1;
    
    /* a(2k) = 0 for k ≥ 1 */
    if ((n & 1ULL) == 0) return (seq_t)0;

    uint64_t x = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    /* factor_u64 fills primes[0..cnt-1] and exps[0..cnt-1] */
    factor_u64(x, primes, exps, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t base = (seq_t)9 * ((seq_t)primes[i] - (seq_t)2);
        for (size_t j = 0; j < exps[i]; ++j) {
            res *= base;
        }
    }
    return res;
}
