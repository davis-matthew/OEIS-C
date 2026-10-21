/*
 * A064834  If n (in base 10) is d_1 d_2 ... d_k then a(n) = Sum_{i = 1..[k/2]
 *          } |d_i - d_{k-i+1}|.
 * https://oeis.org/A064834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{i=1..⌊k/2⌋} |d_i - d_{k-i+1}|, where d_1...d_k are
 *            the decimal digits of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required includes */
#include <stddef.h>
#include <stdint.h>

/* Compute the palindromic deviation of n */
seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t tmp = n;

    /* maximum number of decimal digits for a 64-bit integer (<=20) */
    unsigned digits[20];
    size_t len = 0;

    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp) {
            digits[len++] = (unsigned)(tmp % 10);
            tmp /= 10;
        }
    }

    size_t half = len / 2;
    for (size_t i = 0; i < half; ++i) {
        unsigned d1 = digits[i];
        unsigned d2 = digits[len - 1 - i];
        unsigned diff_u = (d1 > d2) ? d1 - d2 : d2 - d1;
        seq_t diff = (seq_t)diff_u;
        res += diff;
    }

    return res;
}
