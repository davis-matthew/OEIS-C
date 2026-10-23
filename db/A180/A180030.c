/*
 * A180030  Number of n-move paths on a 3 X 3 chessboard of a queen starting
 *          or ending in a corner or side square.
 * https://oeis.org/A180030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 8*a(n-2), a(0)=1, a(1)=6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>           /* for uintptr_t, uint64_t if seq_t is uint8_t */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    /* a and b hold a(k-2) and a(k-1) in each iteration */
    seq_t a_k_2 = (seq_t)1;   /* a(0) */
    seq_t a_k_1 = (seq_t)6;   /* a(1) */
    seq_t a_k  ;

    for (idx_t i = 2; i <= n; ++i) {
        a_k = (seq_t)5 * a_k_1 + (seq_t)8 * a_k_2;
        a_k_2 = a_k_1;
        a_k_1 = a_k;
    }
    return a_k;
}
