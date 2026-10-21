/*
 * A087726  Number of elements X in the matrix ring M_2(Z_n) such that X^2 ==
 *          0 mod n.
 * https://oeis.org/A087726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = product_{p^k || n} ((1 + 3 p² + 2 k (p²-1) + (-1)^k
 *            (p²-1)) * p^{2k-2}) / 4 (sequence is multiplicative; this is the
 *            closed form for a single prime power)
 * Range:     u8 n <= 7, u16 n <= 143, u32 n <= 1000, u64 n <= 1000,
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
        return (seq_t)1;      /* a(1) = 1 */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt;

    /* factorize n */
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < pcnt; ++i) {
        seq_t p   = (seq_t)primes[i];
        seq_t k   = (seq_t)exps[i];
        seq_t p2  = p * p;

        /* term_num = 1 + 3 p² + 2 k (p²-1) + (-1)^k (p²-1) */
        seq_t term_num = 1 + 3 * p2 + 2 * k * (p2 - 1)
                         + ((k & 1) ? -(p2 - 1) : (p2 - 1));

        /* p_pow = p^(2k-2) */
        seq_t p_pow = 1;
        seq_t exp   = 2 * k - 2;          /* zero when k==1, otherwise >=2 */
        for (seq_t j = 0; j < exp; ++j)
            p_pow *= p;

        /* term = term_num * p_pow / 4 (division is exact) */
        seq_t term = term_num * p_pow;
        term /= 4;

        result *= term;
    }

    return result;
}
