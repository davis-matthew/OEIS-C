/*
 * A133482  a(p_1^e_1*p_2^e_2*.....*p_m^e_m) =
 *          (p_1^p_1)^e_1*(p_2^p^2)^e_2*.....*(p_m^p_m)^e_m where
 *          p_1^e_1*p_2^e_2*.....*p_m^e_m is the prime decomposition of n.
 * https://oeis.org/A133482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime powers of p^(p*e)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n <= 1) {
        return 1;
    }
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p_seq = (seq_t)primes[i];
        seq_t exp_seq = (seq_t)primes[i] * (seq_t)exps[i];
        result *= big_pow(p_seq, exp_seq);
    }
    return result;
}
