/*
 * A008492  Expansion of (1-x^10) / (1-x)^10.
 * https://oeis.org/A008492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8064 + 26060*n^2 + 5985*n^4 + 210*n^6 + n^8) / 4032
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 44, u64 n <= 722, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the OEIS sequence A008492 (offset 0).  */
/* For n = 0 the value is defined as 1.  The implementation uses 128‑bit
   integers for intermediate calculations to avoid overflow for all
   values that fit in seq_t. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Use 128‑bit arithmetic for the intermediate calculation. */
    __int128 nn  = n;
    __int128 n2  = nn * nn;          /* n^2   */
    __int128 n4  = n2 * n2;          /* n^4   */
    __int128 n6  = n4 * n2;          /* n^6   */
    __int128 n8  = n6 * n2;          /* n^8   */

    __int128 numer = 8064
                     + 26060 * n2
                     + 5985 * n4
                     + 210   * n6
                     + n8;

    /* division is exact for all n >= 0 */
    return (seq_t)(numer / 4032);
}
