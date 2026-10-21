/*
 * A226782  If n == 0 (mod 2) then a(n) = 0, otherwise a(n) = 4^(-1) in Z/nZ*.
 * https://oeis.org/A226782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n even or n = 1; otherwise a(n) = the modular
 *            inverse of 4 modulo n (1 ≤ a(n) < n)
 * Range:     u8 n <= 340, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return 0;               /* special case */
    if ((n & 1ULL) == 0) return 0;      /* even n → 0 */

    /* n is odd and > 1.  For odd n the inverse of 4 satisfies:
         if n ≡ 3 (mod 4)  →  a(n) = (n + 1) / 4
         if n ≡ 1 (mod 4)  →  a(n) = (3*n + 1) / 4
       These expressions are always integers. */
    if ((n % 4ULL) == 3ULL) {
        /* (n + 1) is divisible by 4 */
        return (seq_t)(((__int128)n + 1) >> 2);
    } else {
        /* n ≡ 1 (mod 4): 3*n + 1 is divisible by 4 */
        return (seq_t)((((__int128)3 * n) + 1) >> 2);
    }
}
