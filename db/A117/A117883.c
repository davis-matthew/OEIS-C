/*
 * A117883  Alternate numbers on a dartboard, read clockwise.
 * https://oeis.org/A117883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = board[(2n−1) mod 20] with board =
 *            {20,1,18,4,13,6,10,15,2,17,3,19,7,16,8,11,14,9,12,5}
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. a(n) = board[1 + 2*((n−1) mod 10)]  (indices 0‑based) */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The dartboard numbers in clockwise order, starting with 20 at index 0. */
    static const seq_t board[20] = {
        20, 1, 18, 4, 13, 6, 10, 15, 2, 17,
        3, 19, 7, 16, 8, 11, 14, 9, 12, 5
    };

    /* n is 1‑based; (n-1)%10 gives the 0‑based position among the 10
       alternating numbers. Multiplying by 2 and adding 1 selects the
       corresponding entry in board (indices 1,3,…,19). */
    if (n == 0) return 0;                     /* undefined for n<1, return 0 */
    idx_t pos = (n - 1) % 10;                 /* 0‑9 */
    size_t idx = (size_t)(2 * pos + 1);        /* 1‑19 */
    return board[idx];
}
