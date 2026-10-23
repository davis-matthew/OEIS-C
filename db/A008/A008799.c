/*
 * A008799  Molien series for group [2,6]+ = 226.
 * https://oeis.org/A008799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n^2+3*n+21+7*(n+1)*(-1)^n)/24)
 * Range:     u8 n <= 73, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a wider type for the intermediate arithmetic to avoid over‑flow
       in the included integer types (seq_t may be 8, 16, 32, 64, 128 bits
       or an arbitrary‑precision type).  The final result fits in seq_t
       for the tests that are run.  */
    unsigned long long nn = (unsigned long long)n;
    unsigned long long numerator;
    if ((n & 1ull) == 0) {
        /* n is even: (-1)^n = 1 */
        numerator = nn * nn + 10ull * nn + 28ull;
    } else {
        /* n is odd: (-1)^n = -1 */
        numerator = nn * nn - 4ull * nn + 14ull;
    }
    unsigned long long res = numerator / 24ull;
    return (seq_t)res;
}
