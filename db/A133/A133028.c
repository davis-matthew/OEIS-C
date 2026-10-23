/*
 * A133028  Even perfect numbers divided by 2.
 * https://oeis.org/A133028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(p-2) * (2^p - 1), where p is the n-th exponent
 *            yielding a Mersenne prime (2^p - 1). This implements OEIS
 *            A133028
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   pow2, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Known exponents of Mersenne primes (first 51). */
    static const idx_t mersenne_exp[] = {
        2,   3,   5,   7,   13,  17,  19,  31,  61,  89,
        107, 127, 521, 607, 1279, 2203, 2281, 3217, 4253, 4423,
        9689, 9941, 11213, 19937, 21701, 23209, 44497, 86243,
        110503, 132049, 216091, 756839, 859433, 1257787, 1398269,
        2976221, 3021377, 6972593, 13466917, 20996011, 24036583,
        25964951, 30402457, 32582657, 37156667, 42543801, 43112609,
        57885161, 74207281, 77232917, 82589933
    };
    const size_t count = sizeof mersenne_exp / sizeof mersenne_exp[0];

    if (n - 1 >= count)
        return 0; /* beyond known perfect numbers */

    idx_t p = mersenne_exp[n - 1];

    seq_t pow_p_minus2 = pow2(p - 2);   /* 2^(p-2) */
    seq_t pow_p       = pow2(p);        /* 2^p */

    seq_t result = pow_p_minus2 * (pow_p - seq_one());
    return result;
}
