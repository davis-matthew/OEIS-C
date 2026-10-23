/*
 * A046030  Numbers whose digits are squares.
 * https://oeis.org/A046030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is obtained by writing (n-1) in base‑4 and mapping digits
 *            0,1,2,3 → 0,1,4,9 to form a decimal number
 * Range:     u8 n <= 32, u16 n <= 768, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for offset 1, but harmless */

    /* Convert n-1 to base 4 */
    idx_t m = n - 1;
    uint8_t buf[64];
    size_t len = 0;
    while (m != 0) {
        buf[len++] = (uint8_t)(m & 3);   /* equivalent to m % 4 */
        m >>= 2;                         /* equivalent to m / 4 */
    }

    /* Map base-4 digits to decimal digits 0,1,4,9 */
    static const seq_t map[4] = {0, 1, 4, 9};

    seq_t res = 0;
    if (len == 0)            /* n == 1 case -> 0 */
        return 0;

    /* Build the decimal number from most significant digit. */
    for (size_t i = len; i-- > 0; )
        res = res * 10 + map[buf[i]];

    return res;
}
