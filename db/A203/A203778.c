/*
 * A203778  a(n) = -24*A015219(n-2)*a(n-1), with a(1) = 2.
 * https://oeis.org/A203778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2 * (-1)^(n+1) * (4n-4)! / (n-1)!
 * Range:     i8 n <= 2, i16 n <= 2, i32 n <= 4, i64 n <= 6, i128 n <= 10,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The product (4n-4)!/(n-1)! equals ∏_{i=n}^{4n-4} i .
 *  The implementation multiplies these integers sequentially,
 *  then multiplies by 2 and applies the sign.
 */

#include <stdlib.h>

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for offset below 1 */

    /* compute the product ∏_{i=n}^{4n-4} i */
    seq_t prod = 1;
    for (idx_t i = n; i <= 4*n - 4; ++i)
        prod = mul_small(prod, (unsigned int)i);

    /* multiply result by 2 */
    prod = mul_small(prod, 2);

    /* apply sign: positive for odd n, negative for even n */
    if ((n & 1) == 0)      /* n even → negative sign */
        prod = -prod;

    return prod;
}
