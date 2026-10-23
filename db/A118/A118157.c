/*
 * A118157  Start with 1 and repeatedly reverse the digits and add 62 to get
 *          the next term.
 * https://oeis.org/A118157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(1)=1; for n>1, a(n)=reverse_decimal(a(n-1))+62
 * Range:     u8 n <= 5, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_decimal.h"
#include <stddef.h>

seq_t a(idx_t n) {
    /* start from n=1 */
    seq_t curr = 1;
    if (n == 1) {
        return curr;
    }

    /* iterate up to n */
    for (idx_t i = 2; i <= n; ++i) {
        curr = reverse_decimal(curr) + (seq_t)62;
    }

    return curr;
}
