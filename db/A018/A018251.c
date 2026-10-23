/*
 * A018251  Divisors of 18.
 * https://oeis.org/A018251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 2^((n-1)&1) * 3^((n-1)>>1)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"

/* Exponentiation by squaring: compute 3^exp in seq_t */
static seq_t pow3(idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)3;

    while (exp) {
        if (exp & (idx_t)1)
            result = seq_mul(result, base);
        exp >>= (idx_t)1;
        if (exp)
            base = seq_mul(base, base);
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;               /* below offset – undefined */

    idx_t t = n - (idx_t)1;           /* shift to zero‑based */
    idx_t b = t & (idx_t)1;           /* exponent of 2, 0 or 1 */
    idx_t c = t >> (idx_t)1;          /* exponent of 3 */

    seq_t res = pow3(c);
    if (b)
        res = seq_mul(res, (seq_t)2); /* multiply by 2 if needed */

    return res;
}
