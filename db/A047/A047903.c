/*
 * A047903  a(1) = 13; for n > 0, a(n+1) = a(n) * sum of digits of a(n).
 * https://oeis.org/A047903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(1) = 13; a(n+1) = a(n) * digit_sum(a(n))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"

/* Returns the n‑th term of OEIS sequence A047903.  */
seq_t a(idx_t n)
{
    /* Start with the first term.  */
    seq_t val = (seq_t)13;
    /* For n==1 the loop is skipped, as required.  */
    for (idx_t i = 1; i < n; ++i) {
        seq_t s = digit_sum(val);
        val = val * s;
    }
    return val;
}
