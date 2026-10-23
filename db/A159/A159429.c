/*
 * A159429  Number of n X n arrays of squares of integers summing to 32.
 * https://oeis.org/A159429
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^32 in (1 + x + x⁴ + x⁹ + x¹⁶ + x²⁵)^(n²)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   mul_trunc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/mul_trunc.h"

seq_t a(idx_t n)
{
    /* For indices below the offset return 0 (unspecified behavior elsewhere). */
    if (n < 2)
        return 0;

    /* Exponent is n². */
    idx_t exp = n * n;

    enum { DEG = 32, CMPLEN = DEG + 1 };

    /* Base polynomial P(x) = 1 + x + x⁴ + x⁹ + x¹⁶ + x²⁵ */
    static const seq_t base_poly[CMPLEN] = {
        1, 1, 0, 0, 1, 0, 0, 0, 0, 1,   /* 0..9 */
        0, 0, 0, 0, 0, 0, 1, 0, 0, 0,   /*10..19*/
        0, 0, 0, 0, 0, 1, 0, 0, 0, 0,   /*20..29*/
        0, 0, 0                     /*30..32*/
    };

    seq_t res[CMPLEN];
    seq_t cur[CMPLEN];
    seq_t tmp[CMPLEN];

    /* Initialize result polynomial to 1. */
    for (size_t i = 0; i < CMPLEN; ++i)
        res[i] = 0;
    res[0] = 1;

    /* Copy base polynomial to cur. */
    for (size_t i = 0; i < CMPLEN; ++i)
        cur[i] = base_poly[i];

    /* Binary exponentiation, truncating to degree 32 at each step. */
    while (exp > 0) {
        if (exp & (idx_t)1) {
            /* tmp = res * cur */
            for (size_t i = 0; i < CMPLEN; ++i)
                tmp[i] = 0;
            mul_trunc(res, cur, tmp, DEG);
            for (size_t i = 0; i < CMPLEN; ++i)
                res[i] = tmp[i];
        }
        /* cur = cur * cur */
        for (size_t i = 0; i < CMPLEN; ++i)
            tmp[i] = 0;
        mul_trunc(cur, cur, tmp, DEG);
        for (size_t i = 0; i < CMPLEN; ++i)
            cur[i] = tmp[i];

        exp >>= 1;
    }

    /* The desired coefficient is the coefficient of x^32. */
    return res[32];
}
