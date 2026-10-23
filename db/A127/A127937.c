/*
 * A127937  Length of longest string of consecutive zeros in the base-7
 *          expansion of 2^n.
 * https://oeis.org/A127937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the longest consecutive run of zeros in the
 *            base-7 representation of 2^n
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* digits stored low-to-high, each 0-6 */
    size_t cap = 1024;
    uint8_t *digits = (uint8_t *)malloc(cap * sizeof(uint8_t));
    if (!digits) return 0; /* allocation failure */
    size_t len = 1;
    digits[0] = 1;          /* 2^0 = 1 */

    for (idx_t k = 0; k < n; ++k) {
        uint8_t carry = 0;
        for (size_t i = 0; i < len; ++i) {
            uint16_t tmp = (uint16_t)digits[i] * 2 + carry;
            digits[i] = (uint8_t)(tmp % 7);
            carry     = (uint8_t)(tmp / 7);
        }
        if (carry) {
            if (len == cap) {
                cap <<= 1;
                uint8_t *tmp = (uint8_t *)realloc(digits, cap * sizeof(uint8_t));
                if (!tmp) { free(digits); return 0; }
                digits = tmp;
            }
            digits[len++] = carry;
        }
    }

    seq_t maxrun = 0;
    seq_t run    = 0;
    for (size_t i = 0; i < len; ++i) {
        if (digits[i] == 0) {
            ++run;
        } else {
            if (run > maxrun) maxrun = run;
            run = 0;
        }
    }
    if (run > maxrun) maxrun = run;

    free(digits);
    return maxrun;
}
