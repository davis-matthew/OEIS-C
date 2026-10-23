/*
 * A107767  a(n) = (1 + 3^n - 2*3^(n/2))/4 if n is even, (1 + 3^n -
 *          4*3^((n-1)/2))/4 if n odd.
 * https://oeis.org/A107767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + 3^n - 2*3^(n/2)) / 4 if n is even a(n) = (1 + 3^n -
 *            4*3^((n-1)/2)) / 4 if n is odd Offset: 1 (i.e. a(1) is the first
 *            term) This implementation uses fast exponentiation for 3^k
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow3_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow3_u.h"

/* External function required by the OEIS framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1; 0 is undefined but harmless */

    seq_t pow3_n   = pow3_u(n);
    seq_t pow3_half;
    seq_t numerator;

    if ((n & 1ULL) == 0) {                 /* even n */
        pow3_half   = pow3_u(n / 2);
        numerator   = (seq_t)1 + pow3_n - (seq_t)2 * pow3_half;
    } else {                               /* odd n */
        pow3_half   = pow3_u((n - 1) / 2);
        numerator   = (seq_t)1 + pow3_n - (seq_t)4 * pow3_half;
    }

    return numerator / 4;
}
