/*
 * A016001  a(n) = (tau(n^6)+5)/6.
 * https://oeis.org/A016001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (tau(n^6)+5)/6
 * Range:     u8 n <= 29, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Factor n into primes.  The helper fills the arrays with
       at most the number of distinct prime factors of n.           */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute the divisor count of n^6: product over primes of (6*e+1). */
    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (seq_t)(6 * exps[i] + 1);
        prod *= term;
    }

    /* Final formula: (prod + 5) / 6.  prod is always divisible by 6. */
    return (prod + 5) / 6;
}
