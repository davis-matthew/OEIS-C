/*
 * A016009  a(n) = (tau(n^13)+12)/13.
 * https://oeis.org/A016009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (tau(n^13)+12)/13
 * Range:     u8 n <= 11, u16 n <= 4619, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    /* factor n into primes and exponents */
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* compute product of (13*exp+1) */
    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (seq_t)(13 * exps[i] + 1);
        prod *= term;
    }

    /* apply formula */
    return (prod + (seq_t)12) / (seq_t)13;
}
