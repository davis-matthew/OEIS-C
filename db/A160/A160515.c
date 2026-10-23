/*
 * A160515  Decimal expansion of the integer (640320^3 + 744)^2 * 70^2 =
 *          337736875876935471466319632507953926400.
 * https://oeis.org/A160515
 *
 * Offset:    39    (a(39) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A160515
   Sequence: decimal expansion of 337736875876935471466319632507953926400
   Offset: 39   (a(39) is the first term)
   Formula: a(n) is the (n‑39)‑th digit of the 39‑digit integer above.
   For n < 39 or n > 77 the value is undefined; this implementation
   returns 0 in those cases. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The 39 decimal digits of the integer
       337736875876935471466319632507953926400   */
    static const seq_t digits[39] = {
        3, 3, 7, 7, 3, 6, 8, 7, 5, 8,
        7, 6, 9, 3, 5, 4, 7, 1, 4, 6,
        6, 3, 1, 9, 6, 3, 2, 5, 0, 7,
        9, 5, 3, 9, 2, 6, 4, 0, 0
    };

    if (n < 39)           /* indices before the offset are undefined */
        return 0;

    size_t idx = (size_t)(n - 39);   /* n >= 39, so subtraction is safe */
    if (idx >= 39)         /* indices beyond the length of the integer */
        return 0;

    return digits[idx];
}
