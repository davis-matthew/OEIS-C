/*
 * A049872  Integers in base 12, written using 2 decimal digits (00, 01, 02,
 *          ...,09, 10, 11) to represent base-12 "digits".
 * https://oeis.org/A049872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = decimal number obtained by writing each base‑12 digit of
 *            n as a two‑digit decimal, concatenated and interpreted as an
 *            integer. For example, n = 22 (base‑12 = 1 10) → "01""10" → 110.
 *            This is equivalent to: produce the least significant base‑12
 *            digit first, then prepend the other digits whil...
 * Range:     u8 n <= 35, u16 n <= 1007, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the zero case directly.  For n > 0, obtain the base‑12
       digits into a stack, then fold them into the final result
       multiplying by 100 and adding the digit value.  The largest
       possible number of base‑12 digits for a 64‑bit index is 18,
       so a fixed small buffer is sufficient.  */
    if (n == 0) return 0;

    uint8_t digits[20];
    int dcnt = 0;
    idx_t temp = n;
    while (temp > 0) {
        digits[dcnt++] = temp % 12;
        temp /= 12;
    }

    seq_t result = 0;
    for (int i = dcnt - 1; i >= 0; --i) {
        result = result * 100 + (seq_t)digits[i];
    }
    return result;
}
