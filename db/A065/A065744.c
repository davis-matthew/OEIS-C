/*
 * A065744  Number of 9's in the decimal expansion of 2^n.
 * https://oeis.org/A065744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Compute a(n) = the number of digit '9' in the decimal expansion of 2^n.
 * The algorithm repeatedly multiplies a decimal digit array by 2,
 * which is efficient enough for n ≤ 10000 (≈ 30 million elementary
 * operations).  No external big‑integer library is required.
 */
#include <stddef.h>   /* for size_t */

/* Maximum number of decimal digits of 2^10000 ≈ 3010. */
#define MAX_DIGITS 3100

seq_t a(idx_t n)
{
    /* digits[i] holds the i‑th decimal digit, least significant first. */
    unsigned char digits[MAX_DIGITS];
    size_t len = 1;          /* current number of used digits */
    digits[0] = 1;           /* 2^0 = 1 */

    /* Perform n multiplications by 2. */
    for (idx_t i = 0; i < n; ++i) {
        unsigned carry = 0;
        for (size_t j = 0; j < len; ++j) {
            unsigned val = (unsigned)digits[j] * 2 + carry;
            digits[j] = (unsigned char)(val % 10);
            carry = val / 10;
        }
        while (carry) {
            digits[len++] = (unsigned char)(carry % 10);
            carry /= 10;
        }
    }

    /* Count the number of '9's. */
    seq_t count = 0;
    for (size_t i = 0; i < len; ++i)
        if (digits[i] == 9)
            ++count;

    return count;
}
