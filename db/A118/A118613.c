/*
 * A118613  Start with 1 and repeatedly reverse the digits and add 27 to get
 *          the next term.
 * https://oeis.org/A118613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; a(n+1) = reverse_decimal(a(n)) + 27 */
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n) {
    if (n == (idx_t)1) return (seq_t)1;
    seq_t cur = (seq_t)1;
    for (idx_t i = (idx_t)1; i < n; ++i) {
        cur = reverse_decimal(cur) + (seq_t)27;
    }
    return cur;
}
