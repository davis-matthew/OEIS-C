/*
 * A118158  Start with 1 and repeatedly reverse the digits and add 63 to get
 *          the next term.
 * https://oeis.org/A118158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; a(n+1) = rev_decimal(a(n)) + 63 */

#include "../utils/reverse_decimal.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* not part of the offset, safety */
    seq_t x = 1;                  /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        x = reverse_decimal(x) + (seq_t)63;
    }
    return x;
}
