/*
 * A174841  Determinant of the symmetric n X n matrix M_n where M_n(j,k) =
 *          n^abs(j-k).
 * https://oeis.org/A174841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1 - n^2)^(n-1)
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 10, i128 n <= 16,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

/* 
 * seq_t a(idx_t n)
 * Compute the determinant of the symmetric n×n matrix M_n with 
 * M_n(j,k) = n^|j-k|.  The closed form is
 *    a(n) = (1 - n^2)^(n-1).
 */
seq_t a(idx_t n) {
    /* Convert n to seq_t and compute base = 1 - n^2 */
    seq_t ns   = (seq_t)n;
    seq_t base = (seq_t)1 - ns * ns;

    /* Exponent is (n-1), cast to seq_t since big_pow expects seq_t */
    seq_t exp = (seq_t)(n - 1);

    return big_pow(base, exp);
}
