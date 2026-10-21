/*
 * A066600  Sum of the digits in the n-th row of Pascal's triangle.
 * https://oeis.org/A066600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = sum of decimal digits of all binomial(n,k), 0 ≤ k ≤ n
 * Range:     u8 n <= 17, u16 n <= 263, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum_array, div_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of all decimal digits of the binomial coefficients
   in the n‑th row of Pascal's triangle.  The algorithm keeps the
   current binomial coefficient as an array of decimal digits so that
   exact arithmetic is possible even when the coefficient does not
   fit in seq_t.  This works for any seq_t (small or arbitrary
   precision) and any idx_t.  */
#include <stdint.h>
#include <stddef.h>
#include "../utils/digit_sum_array.h"
#include "../utils/div_small.h"

#define DIGIT_CAP 5000  /* Enough for n up to about 20000 */

static inline void mul_small(uint8_t *a, size_t *size, unsigned int m)
{
    unsigned int carry = 0;
    for (size_t i = 0; i < *size; ++i) {
        unsigned int tmp = a[i] * m + carry;
        a[i] = tmp % 10;
        carry = tmp / 10;
    }
    while (carry) {
        a[*size] = carry % 10;
        carry /= 10;
        ++(*size);
    }
}

seq_t a(idx_t n)
{
    uint8_t coeff[DIGIT_CAP];
    size_t size = 0;

    /* initial binomial coefficient binom(n,0) = 1 */
    coeff[0] = 1;
    size = 1;

    seq_t sum = 1;  /* digit sum of 1 */

    for (idx_t k = 1; k <= n; ++k) {
        unsigned int mul = (unsigned int)(n - k + 1);
        unsigned int div = (unsigned int)k;

        mul_small(coeff, &size, mul);
        div_small(coeff, &size, div);

        sum += digit_sum_array(coeff, size);
    }

    return sum;
}
