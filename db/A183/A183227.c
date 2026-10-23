/*
 * A183227  a(n) is the base-5 digit sum of 10^n-1.
 * https://oeis.org/A183227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 4*n + A053824(2^n - 1) (base‑5 digit sum of 2^n minus 1)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Computes the base‑5 digit sum of 2^n minus one and adds 4*n.  */
seq_t a(idx_t n)
{
    /* array large enough for 2^n in base 5 up to n ≈ 10 000
       (needs about 4300 digits).  */
    static unsigned char digits[6000];
    size_t len = 1;
    digits[0] = 1;                     /* start with 2^0 = 1 */

    /* multiply by 2, n times */
    for (idx_t i = 0; i < n; ++i) {
        unsigned carry = 0;
        for (size_t j = 0; j < len; ++j) {
            unsigned val = (unsigned)digits[j] * 2 + carry;
            digits[j] = val % 5;
            carry    = val / 5;
        }
        if (carry)                     /* carry is at most 1 */
            digits[len++] = (unsigned char)carry;
    }

    /* digit sum of 2^n */
    seq_t sum = 0;
    for (size_t j = 0; j < len; ++j)
        sum += (seq_t)digits[j];

    /* subtract 1: the least significant base‑5 digit of 2^n is never 0,
       so the sum decreases by exactly 1. */
    sum -= 1ULL;

    /* add the 4*n term */
    return (seq_t)4 * (seq_t)n + sum;
}
