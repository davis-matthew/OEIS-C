/*
 * A102123  Iccanobirt numbers (13 of 15): a(n) = R(R(a(n-1)) + a(n-2) +
 *          R(a(n-3))), where R is the digit reversal function A004086.
 * https://oeis.org/A102123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = R( R(a(n-1)) + a(n-2) + R(a(n-3)) ), a(0) = 0, a(1) = 0,
 *            a(2) = 1
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/rev.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 1;

    seq_t a_nm3 = 0;   /* a(0) */
    seq_t a_nm2 = 0;   /* a(1) */
    seq_t a_nm1 = 1;   /* a(2) */
    seq_t a_n   = 0;   /* current a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t rev_nm1 = rev(a_nm1);
        seq_t rev_nm3 = rev(a_nm3);
        seq_t sum     = rev_nm1 + a_nm2 + rev_nm3;
        a_n          = rev(sum);

        /* roll the window */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
