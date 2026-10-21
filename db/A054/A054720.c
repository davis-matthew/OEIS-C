/*
 * A054720  Number of 5-ary sequences with primitive period n.
 * https://oeis.org/A054720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} μ(d) * 5^{n/d} (OEIS A054720)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   factorize, power5, rec_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factorize.h"   /* factorize(uint64_t n, uint64_t primes[], int exps[]) */
#include "../utils/power5.h"
#include "../utils/rec_divisors.h"

seq_t a(idx_t n)
{
    if (n == 0) /* by definition a(0) = 1 */
        return (seq_t)1;

    /* Factor n */
    uint64_t primes[32];
    int exps[32];
    int pf_count = factorize(n, primes, exps);

    seq_t posSum = 0;
    seq_t negSum = 0;

    rec_divisors(0, 1, 0, true,
                 primes, exps, pf_count,
                 n, &posSum, &negSum);

    seq_t result = posSum;
    result -= negSum;
    return result;
}
