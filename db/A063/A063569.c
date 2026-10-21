/*
 * A063569  6^a(n) is smallest positive power of 6 containing the string 'n'.
 * https://oeis.org/A063569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1002, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the smallest k>0 such that the decimal representation of 6^k
 *  contains the decimal representation of n as a contiguous substring.
 *  The function computes this by repeatedly multiplying the current
 *  power of 6 by 6, converting the result to a decimal string and
 *  testing for the substring.  No memoisation is required because
 *  the exponents needed for n≤10000 are very small (typically <10). */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

seq_t a(idx_t n)
{
    /* Convert n to its decimal string representation */
    char pat[32];
    snprintf(pat, sizeof pat, "%llu", (unsigned long long)n);

    /* Start with 6^0 = "1" */
    char *digits = (char*)malloc(3);        /* digit string + NUL */
    if (!digits) return (seq_t)0;
    digits[0] = '1';
    digits[1] = '\0';
    size_t len = 1;                         /* number of decimal digits */

    idx_t k = 0;                            /* current exponent */

    while (1) {
        /* Multiply current power by 6 */
        unsigned int carry = 0;
        for (int i = (int)len - 1; i >= 0; --i) {
            unsigned int prod = (unsigned int)(digits[i] - '0') * 6 + carry;
            digits[i] = (char)('0' + (prod % 10));
            carry = prod / 10;
        }
        if (carry) {
            /* Need to prepend one new digit.  Allocate space for that digit
             * plus the terminating NUL and shift existing digits right. */
            size_t new_len = len + 2;        /* digits + carry digit + NUL */
            char *tmp = (char*)realloc(digits, new_len);
            if (!tmp) {
                free(digits);
                return (seq_t)0;           /* allocation failure */
            }
            digits = tmp;
            memmove(digits + 1, digits, len + 1); /* include NUL */
            digits[0] = (char)('0' + carry);
            len += 1;
        }

        ++k;                               /* exponent now represents 6^k */

        /* Test for containment of pat */
        if (strstr(digits, pat)) {
            seq_t res = (seq_t)k;
            free(digits);
            return res;
        }
    }
}
