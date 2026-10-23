/*
 * A117677  a(n) = number of divisors of n^2 (excluding 1 and n^2).
 * https://oeis.org/A117677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of divisors of n^2 excluding 1 and n^2, that is
 *            a(n) = ∏(2e_i + 1) − 2 where n = ∏p_i^e_i
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    uint64_t nn = (uint64_t)n;

    /* factor_u64 expects arrays of uint64_t to hold prime factor
       residues and their exponents.  A 64‑bit number can have at most
       64 prime factors (in the worst case all 2’s).  */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;

    factor_u64(nn, pr, ex, &cnt);

    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (seq_t)(2 * (seq_t)ex[i] + 1);
        prod *= term;
    }

    prod -= (seq_t)2;
    return prod;
}
