/*
 * A104759  Concatenation of digits of natural numbers from n down to 1.
 * https://oeis.org/A104759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = reverse of the first n digits of the infinite *
 *            concatenation of 1,2,3,4,5,... (i.e. reverse of
 *            “123456789101112...” * truncated to n digits). This matches a(n)
 *            = A138793(n) mod 10^(n‑1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */
#include <stdint.h>   /* size_t */

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1, but handle gracefully */
        return 0;

    /* Allocate a buffer to hold the first n digits (in order). */
    size_t len = (size_t)n;        /* n ≤ 10000 in test data */
    unsigned char *digits = (unsigned char*)malloc(len);
    if (!digits)
        return 0;                  /* allocation failed – unlikely */

    /* Build the first n digits by appending the decimal digits of each
     * number from 1 upwards until we reach n digits. */
    size_t pos = 0;
    for (idx_t num = 1; pos < len; ++num) {
        /* Extract digits of num into a temporary buffer in reverse order. */
        idx_t tmp = num;
        unsigned char buf[32];
        idx_t tlen = 0;
        do {
            buf[tlen++] = (unsigned char)((tmp % 10) + '0');
            tmp /= 10;
        } while (tmp != 0);

        /* Write them into the main buffer in normal order. */
        for (idx_t k = tlen; pos < len && k > 0; --k) {
            digits[pos++] = buf[k - 1];
        }
    }

    /* Now compute the reversed number: process the stored digits from
     * rightmost to leftmost.  Leading zeros are automatically omitted
     * by the numeric representation. */
    seq_t result = 0;
    for (size_t i = len; i > 0; --i) {
        unsigned char d = digits[i - 1] - '0';
        result = result * 10 + d;
    }

    free(digits);
    return result;
}
