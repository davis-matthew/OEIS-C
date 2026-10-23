/*
 * A170611  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = 26 * 25^(n-1) for n >= 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   seq_mul, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include helper functions from the utils library */
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    if (n == 0U)
        return seq_one();                 /* a(0) = 1 */

    /* For n >= 1: a(n) = 26 * 25^(n-1) */
    return seq_mul(26,                        /* base 26 */
                   seq_pow(25, n - 1));      /* 25^(n-1) */
}
