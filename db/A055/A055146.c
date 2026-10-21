/*
 * A055146  The first n digits of the juxtaposition of the base-5 numbers
 *          converted to decimal.
 * https://oeis.org/A055146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = value of the first n digits of the concatenation of
 * base‑5 numbers (1, 2, 3, 4, 10, 11, 12, …) interpreted as a base‑5 integer.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* number of digits accumulated so far */
    idx_t total = 0;
    /* running value as a base‑5 number, finally returned in decimal */
    seq_t acc = 0;

    /* digits of the current base‑5 number, LSB first (digits[0] is the least
       significant digit).  The largest base‑5 number needed for n up to 10000
       has at most 7 digits, so a small static array is sufficient. */
    uint8_t digits[32];
    int len = 1;      /* current number of digits */
    digits[0] = 1;    /* start with the number 1 */

    while (total < n) {
        /* process the current number: traverse its digits from most to least
           significant, updating the accumulated value. */
        for (int i = len - 1; i >= 0 && total < n; --i) {
            acc = acc * 5 + digits[i];
            ++total;
        }
        if (total >= n) break;

        /* increment the current base‑5 number by 1 */
        int i = 0;
        while (1) {
            if (digits[i] < 4) {
                ++digits[i];
                break;
            } else {
                digits[i] = 0;
                ++i;
                if (i == len) {
                    digits[i] = 1;  /* carry produced a new most‑significant digit */
                    ++len;
                    break;
                }
            }
        }
    }
    return acc;
}
