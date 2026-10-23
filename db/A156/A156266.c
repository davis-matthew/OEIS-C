/*
 * A156266  a(n) = 7^n*Catalan(n).
 * https://oeis.org/A156266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n * Catalan(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   catalan, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t pow7 = pow_uint(7, n);      /* 7^n */
    seq_t cat  = catalan(n);          /* Catalan(n) */
    return pow7 * cat;                /* 7^n * Catalan(n) */
}
