/*
 * A014332  Inverse of 323rd cyclotomic polynomial.
 * https://oeis.org/A014332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n % 323 ∈ [0,16], * 0 if n % 323 ∈ [17,18] ∪
 *            [36,322], * -1 if n % 323 ∈ [19,35]
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    idx_t m = n % 323U;          /* modulus is within 323, fits in idx_t */
    if (m <= 16U)                /* 0..16 */
        return (seq_t)1;
    if (m <= 18U)                /* 17..18 */
        return (seq_t)0;
    if (m <= 35U)                /* 19..35 */
        return (seq_t)-1;
    return (seq_t)0;             /* 36..322 */
}
