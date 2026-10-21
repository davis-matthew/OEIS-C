/*
 * A061824  Multiples of 8 containing only the digits 0, ..., 8.
 * https://oeis.org/A061824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A061824: multiples of 8 containing only digits 0‑8.
   Brute force enumeration of multiples of 8, testing digits for '9'. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* a(0) is undefined, return 0 */

    seq_t i = 0;                         /* current multiple of 8 */
    idx_t count = 0;                     /* number of valid terms seen */

    while (1) {
        /* test whether i contains the digit 9 */
        seq_t x = i;
        bool ok = true;
        while (x) {
            if ((x % 10) == 9) {
                ok = false;
                break;
            }
            x /= 10;
        }

        if (ok) {
            ++count;
            if (count == n)
                return i;
        }
        i += 8;                          /* go to next multiple of 8 */
    }
}
