/*
 * A101102  Fifth partial sums of cubes (A000578).
 * https://oeis.org/A101102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(n+4)*(n+5)*(10 + 3*n*(n+5))/20160
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 45, u64 n <= 767, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    unsigned __int128 N = n;          /* cast once to 128‑bit to avoid overflow */
    unsigned __int128 num = 1;

    num *= N;          /* n         */
    num *= N + 1;      /* n+1       */
    num *= N + 2;      /* n+2       */
    num *= N + 3;      /* n+3       */
    num *= N + 4;      /* n+4       */
    num *= N + 5;      /* n+5       */

    unsigned __int128 inc = (unsigned __int128)10 + (unsigned __int128)3 * N * (N + 5);
    num *= inc;

    num /= 20160;      /* exact division */

    return (seq_t)num;
}
