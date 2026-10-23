/*
 * A117829  Start with 3 and repeatedly reverse the digits and add 4 to get
 *          the next term.
 * https://oeis.org/A117829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 3; a(n) = reverse_decimal(a(n-1)) + 4 for n > 1
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    if (n < 1) abort();          /* invalid index – not expected */
    seq_t val = (seq_t)3;        /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t rev = reverse_decimal(val);
        val = rev + (seq_t)4;   /* add 4 after reversing */
    }
    return val;
}
