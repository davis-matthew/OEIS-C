/*
 * A025727  Index of 7^n within the sequence of the numbers of the form
 *          7^i*10^j.
 * https://oeis.org/A025727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Σ_{k=1..n} digits(7^k) * where digits(x) is the
 *            number of decimal digits of x. * Since digits(7^k) =
 *            ⌈k·log10(7)⌉ for k ≥ 1, this is equivalent to * the OEIS
 *            definition a(n) = 1 + Σ_{k=0..n} ⌈k·log10(7)⌉
 * Range:     u8 n <= 23, u16 n <= 392, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset: a(0) = 1 */
    if (n == 0) return (seq_t)1;

    /* Allocate a decimal digit buffer.
     * Grow it geometrically while multiplying by 7 each step. */
    size_t cap = 16;
    uint8_t *digits = (uint8_t *)malloc(cap);
    if (!digits) return (seq_t)0;   /* allocation failure → 0 default */

    size_t len = 1;           /* number of digits in 7^k (starting with 7^0 = 1) */
    digits[0] = 1;            /* 7^0 = 1 */

    seq_t sum = 0;            /* accumulate Σ digits(7^k) for k = 1..n */

    for (idx_t k = 1; k <= n; ++k) {
        /* Multiply the current decimal number by 7 */
        int carry = 0;
        for (size_t i = 0; i < len; ++i) {
            int prod = digits[i] * 7 + carry;
            digits[i] = prod % 10;
            carry     = prod / 10;
        }
        while (carry > 0) {
            if (len == cap) {
                cap *= 2;
                digits = (uint8_t *)realloc(digits, cap);
                if (!digits) return (seq_t)0; /* allocation failure → 0 default */
            }
            digits[len++] = carry % 10;
            carry /= 10;
        }

        /* Now len = digits(7^k) */
        sum += (seq_t)len;
    }

    free(digits);

    /* Return a(n) = 1 + Σ_{k=1..n} digits(7^k) */
    return sum + (seq_t)1;
}
