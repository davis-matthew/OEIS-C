/*
 * A158324  Successive powers of two, represented as binary coded decimal.
 *          (0x1, 0x2, 0x4, 0x8, 0x16, 0x32, etc.)
 * https://oeis.org/A158324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = binary concatenation of BCD encodings of decimal digits
 *            of 2^(n‑1)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset handling – value is undefined for n<1 */

    /* Estimate maximum number of decimal digits of 2^(n-1) */
    double d = (double)(n - 1) * 0.3010299956639812; /* log10(2) */
    size_t max_digits = (size_t)(d) + 5;              /* safety margin */

    /* Allocate array for decimal digits (least significant first) */
    uint8_t *digits = (uint8_t*)malloc(max_digits);
    if (!digits) return 0; /* allocation failure – unlikely in tests */

    size_t len = 1;                /* current number of digits */
    digits[0] = 1;                 /* start with 1 = 2^0 */

    /* Compute decimal representation of 2^(n-1) by repeated doubling */
    for (idx_t k = 1; k < n; ++k) {
        uint16_t carry = 0;
        for (size_t i = 0; i < len; ++i) {
            uint16_t prod = (uint16_t)digits[i] * 2 + carry;
            digits[i] = (uint8_t)(prod % 10);
            carry = prod / 10;
        }
        if (carry) {
            /* carry is always <= 1 for binary multiplication, but treat generically */
            digits[len++] = (uint8_t)carry;
        }
    }

    /* Convert decimal digits to BCD-coded binary number */
    seq_t res = 0;
    for (size_t i = len; i-- > 0;) {
        res = (res << 4) | (seq_t)(digits[i]);
    }

    free(digits);
    return res;
}
