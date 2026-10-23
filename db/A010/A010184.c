/*
 * A010184  Continued fraction for sqrt(119).
 * https://oeis.org/A010184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 500, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A010184:  Continued fraction for sqrt(119).
   a(0) = 10.
   For n ≥ 1, a(n) is multiplicative with
       a(2)      = 9,
       a(2^e)    = 20 for e ≥ 2,
       a(p^e)    = 1 for odd prime p.
   This yields the simple periodic rule:
       if n is odd      → a(n) = 1
       else if n%4==0   → a(n) = 20
       else             → a(n) = 9
*/
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) {
        return 10;
    }
    if (n & 1ULL) {            /* odd */
        return 1;
    }
    if ((n & 3ULL) == 0ULL) {  /* divisible by 4 */
        return 20;
    }
    return 9;                  /* even but not divisible by 4 */
}
