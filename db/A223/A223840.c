/*
 * A223840  Number of 4 X n 0..1 arrays with rows, diagonals and antidiagonals
 *          unimodal and columns nondecreasing.
 * https://oeis.org/A223840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 60, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (n^8 - 4n^7 + 266n^6 + 1568n^5 + 3689n^4 + 103124n^3
 *                 + 16204n^2 - 245808n + 524160) / 40320   (n ≥ 3)
 * Initial values: a(1) = 5, a(2) = 25
 * For n < 1 behaviour undefined.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle n = 1, 2 explicitly */
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)25;
    if (n < 1) return (seq_t)0;  /* safe fallback */

    /* compute with 128‑bit intermediate to avoid overflow for large n */
    __int128 tn = ( __int128 ) n;                      /* n          */
    __int128 n2 = tn * tn;                             /* n^2        */
    __int128 n3 = n2 * tn;                             /* n^3        */
    __int128 n4 = n2 * n2;                             /* n^4        */
    __int128 n5 = n4 * tn;                             /* n^5        */
    __int128 n6 = n3 * n3;                             /* n^6 (= n^3*n^3) */
    __int128 n7 = n6 * tn;                             /* n^7        */
    __int128 n8 = n4 * n4;                             /* n^8        */

    /* combine terms according to the polynomial */
    __int128 num = 0;
    num +=  ( __int128 )1   * n8;
    num -=  ( __int128 )4   * n7;
    num +=  ( __int128 )266 * n6;
    num +=  ( __int128 )1568* n5;
    num +=  ( __int128 )3689* n4;
    num +=  ( __int128 )103124* n3;
    num +=  ( __int128 )16204* n2;
    num -=  ( __int128 )245808* tn;
    num +=  ( __int128 )524160;                         /* constant term */

    /* division exact  */
    return (seq_t)(num / 40320);
}
