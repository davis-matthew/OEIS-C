/*
 * A118220  Start with 1 and repeatedly reverse the digits and add 72 to get
 *          the next term.
 * https://oeis.org/A118220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   reverse_decimal_custom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1,
 *           a(n) = reverse_decimal(a(n-1)) + 72   for n > 1
 * where reverse_decimal(x) is x written in base‑10 with its digits
 * reversed (e.g. reverse_decimal(123) = 321). */
#include <stdint.h>
#include "../utils/reverse_decimal_custom.h"

/* Compute the n-th term of sequence A118220. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;            /* sequence starts at n = 1 */

    seq_t val = (seq_t)1;           /* a(1) = 1 */
    for (idx_t i = (idx_t)1; i < n; ++i)
        val = reverse_decimal_custom(val) + (seq_t)72;  /* recurrence */

    return val;
}
