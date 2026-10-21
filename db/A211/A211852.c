/*
 * A211852  Number of nonnegative integer arrays of length 2n+7 with new
 *          values 0 upwards introduced in order, no three adjacent elements
 *          all unequal, and containing the value n+1.
 * https://oeis.org/A211852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (142*n^5 + 1685*n^4 + 8040*n^3 + 19735*n^2 + 25838*n +
 *            15120) / 120 for n >= 1
 * Range:     u8 none, u16 n <= 6, u32 n <= 79, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit unsigned arithmetic for the intermediate product
       to avoid overflow for n up to ~10 000.  This works for
       64‑bit builds (the low 64 bits are kept) and for arbitrary
       precision builds (the cast creates the exact value). */
    __int128 N  = (__int128)n;      /* n */
    __int128 N2 = N * N;            /* n^2 */
    __int128 N3 = N2 * N;           /* n^3 */
    __int128 N4 = N3 * N;           /* n^4 */
    __int128 N5 = N4 * N;           /* n^5 */

    __int128 num = 142  * N5
                 + 1685 * N4
                 + 8040 * N3
                 + 19735 * N2
                 + 25838 * N
                 + 15120;          /* constant term */

    return (seq_t)(num / 120);
}
