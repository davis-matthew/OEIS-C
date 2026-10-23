/*
 * A188839  Number of n X 5 binary arrays without the pattern 0 1 diagonally
 *          or vertically.
 * https://oeis.org/A188839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^5 + 35n^4 + 445n^3 + 2365n^2 + 3754n - 3960) / 120 for
 *            n > 2; a(1) = 32, a(2) = 144
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)32;
    if (n == 2) return (seq_t)144;

    seq_t N = (seq_t)n;
    unsigned __int128 n128 = (unsigned __int128)N;

    /* Compute powers of n */
    unsigned __int128 n2 = n128 * n128;           /* n^2 */
    unsigned __int128 n3 = n2 * n128;             /* n^3 */
    unsigned __int128 n4 = n3 * n128;             /* n^4 */
    unsigned __int128 n5 = n4 * n128;             /* n^5 */

    unsigned __int128 num = 0;
    num += n5;                                   /* n^5      */
    num += (unsigned __int128)35 * n4;           /* 35 n^4   */
    num += (unsigned __int128)445 * n3;          /* 445 n^3  */
    num += (unsigned __int128)2365 * n2;         /* 2365 n^2 */
    num += (unsigned __int128)3754 * n128;       /* 3754 n   */
    num -= 3960;                                 /* -3960    */

    num /= 120;                                   /* divide by 120 */

    return (seq_t)num;
}
