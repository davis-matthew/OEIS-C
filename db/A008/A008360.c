/*
 * A008360  Crystal ball sequence for D_7 lattice.
 * https://oeis.org/A008360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (2*n+1)*(242*n^6+726*n^5+1856*n^4+2502*n^3+2207*n^2+1077*n+315)/315
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 21, u64 n <= 531,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) for any non‑negative idx_t n.  
 * Works for built‑in integer seq_t types (8/16/32/64/128 bits) 
 * as well as for arbitrary‑precision seq_t classes that support the usual
 * arithmetic operators and construction from integral values.
 */
seq_t a(idx_t n)
{
    /* use unsigned __int128 for intermediates to avoid overflow
     * for 64‑bit seq_t builds.  For arbitrary‑precision seq_t
     * the conversion at the end creates the correct big integer. */
    unsigned __int128 nn = n;

    unsigned __int128 p = 242;
    p = p * nn + 726;
    p = p * nn + 1856;
    p = p * nn + 2502;
    p = p * nn + 2207;
    p = p * nn + 1077;
    p = p * nn + 315;

    unsigned __int128 num = (2 * nn + 1) * p;

    /* The division is exact by the formula. */
    seq_t result = (seq_t)(num / 315);
    return result;
}
