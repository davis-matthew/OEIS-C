/*
 * A092350  a(n) = sigma_3(n) - sigma_2(n) - sigma_1(n).
 * https://oeis.org/A092350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = sigma_3(n) - sigma_2(n) - sigma_1(n)
 * Range:     i8 n <= 5, i16 n <= 31, i32 n <= 1223, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    if (n < 1)                  /* index below the offset is undefined */
        return 0;

    seq_t s3 = sigma_k(n, 3);   /* σ₃(n)  */
    seq_t s2 = sigma_k(n, 2);   /* σ₂(n)  */
    seq_t s1 = sigma_k(n, 1);   /* σ₁(n)  */

    return s3 - s2 - s1;
}
