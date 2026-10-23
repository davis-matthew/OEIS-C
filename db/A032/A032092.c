/*
 * A032092  Number of reversible strings with n-1 beads of 2 colors. 5 beads
 *          are black. String is not palindromic.
 * https://oeis.org/A032092
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n >= 7 a(n) = binom(n-1,5)/2 if n is odd a(n) = (
 *            binom(n-1,5) - binom(n/2-1,2) ) / 2 if n is even where
 *            binom(m,k) = m choose k
 * Range:     u8 n <= 11, u16 n <= 26, u32 n <= 254, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom_n_5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_n_5.h"

seq_t a(idx_t n) {
    if (n < 7) return 0; /* value undefined for n < 7 */

    if (n & 1) { /* odd n */
        return binom_n_5(n) / 2;
    } else { /* even n */
        seq_t bin5 = binom_n_5(n);                     /* C(n-1,5) */
        idx_t m = n / 2 - 1;                           /* m = n/2 - 1 */
        seq_t bin2 = (seq_t)m * (m - 1) / 2;           /* C(m,2) */
        return (bin5 - bin2) / 2;
    }
}
