/*
 * A122162  Coefficient of q-series for constant term of Tate curve.
 * https://oeis.org/A122162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5 sigma_3(n) + 7 sigma_5(n)) / 12
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 92, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    /* Ensure valid input (offset is 1) */
    if (n == 0) return (seq_t)0;

    seq_t s3 = sigma_k(n, (uint8_t)3);
    seq_t s5 = sigma_k(n, (uint8_t)5);

    seq_t num = s3 * (seq_t)5 + s5 * (seq_t)7;
    return num / (seq_t)12;
}
