/*
 * A136565  a(n) = sum of the distinct values making up the exponents in the
 *          prime-factorization of n.
 * https://oeis.org/A136565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the distinct exponents in the prime factorisation
 *            of n
 * Range:     u8 n <= 8683, u16 n <= 8683, u32 n <= 8683, u64 n <= 8683,
 *            u128 n <= 8683, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;

    /* buffers are large enough for any 64‑bit number */
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        bool distinct = true;
        for (size_t j = 0; j < i; ++j)
            if (exps[i] == exps[j]) {
                distinct = false;
                break;
            }
        if (distinct)
            sum += (seq_t)exps[i];
    }
    return sum;
}
