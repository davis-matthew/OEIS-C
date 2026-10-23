/*
 * A016012  a(n) = (tau(n^n)+n-1)/n.
 * https://oeis.org/A016012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (tau(n^n)+n-1)/n
 * Range:     u8 n <= 11, u16 n <= 41, u32 n <= 1259, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0u) {
        return (seq_t)0;
    }
    uint64_t n64 = (uint64_t)n;
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(n64, primes, exps, &cnt);

    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = ((seq_t)exps[i]) * (seq_t)n + (seq_t)1;
        prod = prod * term;
    }

    seq_t n_seq = (seq_t)n;
    seq_t result = (prod + n_seq - (seq_t)1) / n_seq;
    return result;
}
