/*
 * A162813  a(n) = 3*a(n-2) for n > 2; a(1) = 5, a(2) = 3.
 * https://oeis.org/A162813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) for n>2; a(1)=5, a(2)=3
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 1) return 5;
    if (n == 2) return 3;
    if ((n & 1) == 0) {                      /* even n: a(n) = 3^(n/2) */
        return pow3(n >> 1);
    } else {                                  /* odd n: a(n) = 5 * 3^((n‑1)/2) */
        return ((seq_t)5) * pow3((n - 1) >> 1);
    }
}
