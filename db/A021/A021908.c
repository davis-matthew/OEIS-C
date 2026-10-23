/*
 * A021908  Decimal expansion of 1/904.
 * https://oeis.org/A021908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes a(n) = the nth digit after the decimal point of 1/904.
   The digit is obtained by performing long division of 1 by 904.
   The algorithm is O(n) per call but uses only O(1) extra space.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    uint64_t remainder = 1;   /* current remainder, always fits in 904 */
    seq_t digit = 0;          /* current digit to return */

    for (idx_t i = 0; i <= n; ++i) {
        /* digit = floor((remainder * 10) / 904) */
        digit = (seq_t)((remainder * 10ULL) / 904ULL);
        /* next remainder = (remainder * 10) % 904 */
        remainder = (remainder * 10ULL) % 904ULL;
    }
    return digit;
}
