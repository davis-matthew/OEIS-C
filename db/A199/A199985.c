/*
 * A199985  Numbers with digital product = 5.
 * https://oeis.org/A199985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers with digital product = 5 (decimal numbers composed
 *            solely of the digits 1 and 5, containing exactly one digit 5,
 *            sorted in increasing numeric order). Thus a(1)=5, a(2)=15,
 *            a(3)=51, a(4)=115,
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 54, u64 n <= 209, u128 n <= 779,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Find the length L of the nth term:
       count(L) = L numbers of length L (one 5 and L-1 ones). */
    idx_t cum = 0;      /* cumulative count of numbers with length < L */
    idx_t L = 1;        /* current length being examined */

    while (cum + L < n) {
        cum += L;
        ++L;
    }

    /* offset within current length: 1 <= offset <= L */
    idx_t offset = n - cum;          /* 1-indexed */
    idx_t pos_of_five = L - offset;  /* position of the single '5'
                                       from the left (0‑based) */

    /* Build the decimal number digit by digit. */
    seq_t res = 0;
    for (idx_t i = 0; i < L; ++i) {
        res = res * 10 + (i == pos_of_five ? 5 : 1);
    }
    return res;
}
