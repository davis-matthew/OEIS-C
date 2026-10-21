/*
 * A085941  Duplicate of A054842.
 * https://oeis.org/A085941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 3^{floor(n/10)} * 2^{n mod 10}
 * Range:     i8 n <= 6, i16 n <= 22, i32 n <= 22, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   pow2, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    idx_t k = n / 10;      /* floor(n/10)   */
    idx_t r = n % 10;      /* n mod 10      */
    seq_t res = pow2(r);   /* 2^{n mod 10}  */
    res = res * pow_int((seq_t)3, k); /* *3^{floor(n/10)} */
    return res;
}
