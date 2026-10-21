/*
 * A079913  Solution to the Dancing School Problem with 8 girls and n+8 boys:
 *          f(8,n).
 * https://oeis.org/A079913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^8 -20*n^7 +238*n^6 -1820*n^5 +9625*n^4 -35000*n^3
 *            +84448*n^2 -122240*n +80680 for n >= 6 a(0)=1, a(1)=9, a(2)=221,
 *            a(3)=2227, a(4)=15458, a(5)=80196, a(6)=334072, a(7)=1173240,
 *            a(8)=3598120
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 18, u64 n <= 258, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#ifdef __SIZEOF_INT128__
  typedef __int128 int128_t;
  typedef unsigned __int128 uint128_t;
#else
  #error "Compiler does not support 128-bit integer type."
#endif

seq_t a(idx_t n)
{
    /* first 6 values, n = 0 … 5 */
    static const seq_t init[6] = { 1, 9, 221, 2227, 15458, 80196 };

    if (n < 6)
        return init[n];

    /* compute n^k for k = 2 … 8 using 128‑bit arithmetic */
    uint128_t nn = (uint128_t)n;
    uint128_t n2 = nn * nn;                 /* n^2 */
    uint128_t n3 = n2 * nn;                 /* n^3 */
    uint128_t n4 = n3 * nn;                 /* n^4 */
    uint128_t n5 = n4 * nn;                 /* n^5 */
    uint128_t n6 = n5 * nn;                 /* n^6 */
    uint128_t n7 = n6 * nn;                 /* n^7 */
    uint128_t n8 = n7 * nn;                 /* n^8 */

    /* signed 128‑bit result */
    int128_t res = (int128_t)n8
                   - (int128_t)20 * (int128_t)n7
                   + (int128_t)238 * (int128_t)n6
                   - (int128_t)1820 * (int128_t)n5
                   + (int128_t)9625 * (int128_t)n4
                   - (int128_t)35000 * (int128_t)n3
                   + (int128_t)84448 * (int128_t)n2
                   - (int128_t)122240 * (int128_t)nn
                   + (int128_t)80680;

    return (seq_t)res;
}
