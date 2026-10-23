/*
 * A115504  Number of monic irreducible polynomials of degree 1 in
 *          GF(2^n)[x1,x2,x3,x4,x5].
 * https://oeis.org/A115504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + 4^n + 8^n + 16^n + 32^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;       /* offset is 1; a(0) is irrelevant */

    seq_t res = pow2(n);            /* 2^n */
    res += pow4(n);                 /* 4^n = 2^(2n) */
    res += pow2(3 * n);             /* 8^n = 2^(3n) */
    res += pow2(4 * n);             /* 16^n = 2^(4n) */
    res += pow2(5 * n);             /* 32^n = 2^(5n) */

    return res;
}
