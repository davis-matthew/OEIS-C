/*
 * A008885  Aliquot sequence starting at 30.
 * https://oeis.org/A008885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   aliquot_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 30; a(n+1) = sigma(a(n)) – a(n) (proper divisor sum) */
#include <stdint.h>
#include "../utils/aliquot_part.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 30;
    seq_t cur = 30;
    for (idx_t i = 1; i <= n; ++i) {
        if (cur == 0)
            return 0;
        /* The current value fits in uint64_t – all intermediate sums
           also fit in seq_t (which is at least 64 bits). */
        cur = aliquot_part((uint64_t)cur);
    }
    return cur;
}
