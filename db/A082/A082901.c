/*
 * A082901  a(n) = A082895(n)-A000203(n); the distance from sigma(n) to that
 *          multiple of n which is closest to sigma(n), positive terms for
 *          cases where the closest multiple is after sigma(n), and negative
 *          terms where it is before sigma(n). In case of ties, a positive
 *          term is selected.
 * https://oeis.org/A082901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = n*floor[(floor(n/2)+sigma(n))/n] - sigma(n)
 * Range:     i8 n <= 59, i16 n <= 9763, i32 n <= 9763, i64 n <= 9763,
 *            i128 n <= 9763, big any n
 * Helpers:   sigma_of_num
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of_num.h"

seq_t a(idx_t n)
{
    /* compute σ(n) */
    seq_t sigma = sigma_of_num(n);

    /* compute k = floor[(σ(n) + floor(n/2)) / n] */
    seq_t k = (sigma + (seq_t)(n / 2)) / (seq_t)n;

    /* a(n) = n*k - σ(n) */
    return (seq_t)n * k - sigma;
}
