/*
 * A128767  Number of inequivalent n-colorings of the 4D hypercube under the
 *          full orthogonal group of the cube (of order 2^4*4! = 384).
 * https://oeis.org/A128767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (48*n^2 + 180*n^4 + 48*n^6 + 83*n^8 + 12*n^10 + 12*n^12 +
 *            n^16) / 384
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for intermediate values to avoid overflow
       when seq_t is 64‑bit.  The final result fits into seq_t. */
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 n2  = N * N;          /* n^2    */
    unsigned __int128 n4  = n2 * n2;        /* n^4    */
    unsigned __int128 n6  = n4 * n2;        /* n^6    */
    unsigned __int128 n8  = n4 * n4;        /* n^8    */
    unsigned __int128 n10 = n8 * n2;        /* n^10   */
    unsigned __int128 n12 = n8 * n4;        /* n^12   */
    unsigned __int128 n16 = n8 * n8;        /* n^16   */

    unsigned __int128 poly =
        (unsigned __int128)48  * n2  +
        (unsigned __int128)180 * n4  +
        (unsigned __int128)48  * n6  +
        (unsigned __int128)83  * n8  +
        (unsigned __int128)12  * n10 +
        (unsigned __int128)12  * n12 +
        n16;

    return (seq_t)(poly / (unsigned __int128)384);
}
