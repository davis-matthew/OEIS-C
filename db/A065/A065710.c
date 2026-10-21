/*
 * A065710  Number of 2's in the decimal expansion of 2^n.
 * https://oeis.org/A065710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7138, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A065710: Number of 2's in the decimal expansion of 2^n
   Computation method: simulate multiplication of the decimal
   representation of 2^n.  All digits are stored in an array
   of uint8_t with base‑10 little‑endian layout.
   After reaching 2^n, scan the array to count digits equal to 2.
   Complexity: O(n²) digit operations, ~1.5·10⁷ for n = 10⁴. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 2^0 = 1 has no digit 2 */
    if (n == 0) return (seq_t)0;

    /* Rough upper bound on decimal digits of 2^n:
       floor(n*log10(2)) + 1,  log10(2) ≈ 0.30103.  Add a few spare slots. */
    size_t max_digits = (size_t)(n * 301 / 1000) + 5;
    uint8_t *buf = (uint8_t*)malloc(max_digits);
    if (!buf) return (seq_t)0;            /* allocation failure → 0 */

    size_t len = 1;
    buf[0] = 1;                           /* start with 2^0 = 1 */

    for (idx_t i = 0; i < n; ++i) {
        uint8_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint16_t tmp = (uint16_t)buf[j] * 2 + carry;
            buf[j] = (uint8_t)(tmp % 10);
            carry   = (uint8_t)(tmp / 10);
        }
        if (carry) {                      /* carry is 1 for base 10 */
            if (len >= max_digits) {      /* safety check, should not happen */
                uint8_t *new_buf = (uint8_t*)realloc(buf, max_digits * 2);
                if (!new_buf) { free(buf); return (seq_t)0; }
                buf = new_buf;
                max_digits *= 2;
            }
            buf[len++] = carry;
        }
    }

    /* Count digits equal to 2 */
    seq_t count = 0;
    for (size_t j = 0; j < len; ++j)
        if (buf[j] == 2) count += (seq_t)1;

    free(buf);
    return count;
}
