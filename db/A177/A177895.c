/*
 * A177895  Trace of the square matrix whose rows are the cyclic permutations
 *          of the digits of n.
 * https://oeis.org/A177895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the trace of the matrix whose rows are the cyclic
 *            permutations * of the decimal digits of n. If n has an odd
 *            number of digits the trace equals * the sum of all digits; if it
 *            has an even number of digits the trace equals * twice the sum of
 *            the digits in the even positions (0‑based)
 * Range:     u8 n <= 147, u16 n <= 147, u32 n <= 147, u64 n <= 147,
 *            u128 n <= 147, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* collect digits in reverse order */
    unsigned char rev[20];
    idx_t len = 0;
    idx_t tmp = n;
    do {
        rev[(size_t)len++] = tmp % 10;
        tmp /= 10;
    } while (tmp);

    /* build digits in normal order */
    unsigned char digits[20];
    for (idx_t i = 0; i < len; ++i)
        digits[i] = rev[len - 1 - i];

    /* compute the trace */
    seq_t sum = 0;
    if (len % 2 == 1) {
        for (idx_t i = 0; i < len; ++i)
            sum += digits[i];
    } else {
        for (idx_t i = 0; i < len; i += 2)
            sum += digits[i];
        sum *= (seq_t)2;
    }
    return sum;
}
