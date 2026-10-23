/*
 * A170825  a(n) is the product of the distinct primes of form 6*k-1 that
 *          divide n.
 * https://oeis.org/A170825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product of the distinct primes of form 6*k-1 that divide
 *            n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* factor_u64 expects a uint64_t, but idx_t is uint64_t, so cast is safe */
    uint64_t num = (uint64_t)n;
    /* Maximum number of distinct prime factors for 64‑bit n is < 64 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(num, primes, exps, &cnt);

    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        if ((p & 1) && (p % 6 == 5)) {  /* p mod 6 == 5 is same as 6k-1 */
            prod *= (seq_t)p;
        }
    }
    return prod;
}
