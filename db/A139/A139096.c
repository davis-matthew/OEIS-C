/*
 * A139096  Infraperfect numbers: a(n) = 2^(2*p - 1) - 2^p, where p is
 *          A000043(n).
 * https://oeis.org/A139096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*A000043(n)-1) - 2^A000043(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A(n) computes the nth term of OEIS A139096, the differ­ence
 *  between the nth even perfect number and the nth even super‑perfect
 *  number.  A000043(n) is the nth exponent of a Mersenne prime; here
 *  we store the known exponents in a static table.
 *  For n beyond the number of known exponents, 0 is returned. */

#include <stddef.h>
#include "../utils/pow2.h"   /* pow2(idx_t exp) → seq_t */

static const idx_t mersenne_exponents[] = {
    2,   3,   5,   7,   13,  17,  19,  31,  61,  89,
    107, 127, 521, 607, 1279, 2203, 2281, 3217, 4253, 4423,
    9689, 9941, 11213, 19937, 21701, 23209, 44497, 86243, 110503, 132049
};

static const size_t MERSENNE_COUNT = sizeof(mersenne_exponents)/sizeof(mersenne_exponents[0]);

seq_t a(idx_t n)
{
    if (n < 1 || (size_t)n > MERSENNE_COUNT) return 0;

    idx_t p = mersenne_exponents[n-1];

    seq_t big1 = pow2((idx_t)(2*p - 1));
    seq_t big2 = pow2(p);

    big1 -= big2;
    return big1;
}
