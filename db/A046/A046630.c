/*
 * A046630  Number of cubic residues mod 2^n.
 * https://oeis.org/A046630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A046630 – Number of cubic residues mod 2^n
    Formula: a(n) = ceil(2^(n+2) / 7)
*/
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^(n+2) using the provided power routine */
    seq_t p = pow2(n + 2);

    /* ceil division by 7: (x+6)/7 for integer x */
    return (p + (seq_t)6) / (seq_t)7;
}
