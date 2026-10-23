/*
 * A188841  Number of n X 7 binary arrays without the pattern 0 1 diagonally
 *          or vertically.
 * https://oeis.org/A188841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^7 + 70 n^6 + 2002 n^5 + 29680 n^4 + 235669 n^3 +
 *            887530 n^2 + 679608 n - 2882880) / 5040 for n > 4 (exact integer
 *            for every integer n >= 1)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 70, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const seq_t pre[4] = {128, 987, 3280, 7753};

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* out of range – undefined */
    if (n <= 4) return pre[n - 1];

    /* Use 128‑bit arithmetic for intermediate values */
    __int128 NN = (__int128) n;
    __int128 n2 = NN * NN;          /* n^2   */
    __int128 n3 = n2 * NN;          /* n^3   */
    __int128 n4 = n3 * NN;          /* n^4   */
    __int128 n5 = n4 * NN;          /* n^5   */
    __int128 n6 = n5 * NN;          /* n^6   */
    __int128 n7 = n6 * NN;          /* n^7   */

    __int128 num =  n7
                  + (__int128)70   * n6
                  + (__int128)2002 * n5
                  + (__int128)29680 * n4
                  + (__int128)235669 * n3
                  + (__int128)887530 * n2
                  + (__int128)679608 * NN
                  - (__int128)2882880;

    return (seq_t)(num / 5040);
}
