/*
 * A116824  Number of permutations of length n which avoid the patterns 1234,
 *          3412, 3421.
 * https://oeis.org/A116824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n^6 - 78*n^5 + 460*n^4 - 1500*n^3 + 2953*n^2 - 2922*n
 *            + 1440)/360
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* behavior below offset is irrelevant */
    __int128 nn = (unsigned __int128)n;  /* use 128‑bit integer for safety          */
    __int128 n1 = nn;
    __int128 n2 = n1 * n1;
    __int128 n3 = n2 * n1;
    __int128 n4 = n3 * n1;
    __int128 n5 = n4 * n1;
    __int128 n6 = n5 * n1;
    __int128 num = 7 * n6 - 78 * n5 + 460 * n4 - 1500 * n3 + 2953 * n2 - 2922 * n1 + 1440;
    __int128 val = num / 360;
    return (seq_t)val;
}
