/*
 * A054472  Number of ways to color faces of an icosahedron using at most n
 *          colors.
 * https://oeis.org/A054472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = (1/60)*(n^20 + 15*n^10 + 20*n^8 + 24*n^4)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 11, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic when seq_t is small to avoid overflow
       for all n that fit in the target type.  For larger seq_t types
       (arbitrary precision) we compute with seq_t itself. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 nn = n;
        unsigned __int128 n2  = nn * nn;          /* n^2   */
        unsigned __int128 n4  = n2 * n2;          /* n^4   */
        unsigned __int128 n8  = n4 * n4;          /* n^8   */
        unsigned __int128 n10 = n8 * n2;          /* n^10  */
        unsigned __int128 n20 = n10 * n10;        /* n^20  */

        unsigned __int128 term = n20
                               + (unsigned __int128)15 * n10
                               + (unsigned __int128)20 * n8
                               + (unsigned __int128)24 * n4;
        term /= 60;
        return (seq_t)term;
    } else {
        seq_t ns  = (seq_t)n;
        seq_t n2  = ns * ns;                      /* n^2   */
        seq_t n4  = n2 * n2;                      /* n^4   */
        seq_t n8  = n4 * n4;                      /* n^8   */
        seq_t n10 = n8 * n2;                      /* n^10  */
        seq_t n20 = n10 * n10;                    /* n^20  */

        seq_t term = n20
                   + (seq_t)15 * n10
                   + (seq_t)20 * n8
                   + (seq_t)24 * n4;
        term /= (seq_t)60;
        return term;
    }
}
