/*
 * A033132  Base-5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,1,0.
 * https://oeis.org/A033132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ⌊(15/62) * 5^n⌋, n ≥ 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

/* Return the n‑th term of sequence A033132.
   For n = 0 the value is undefined; we return 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Compute 5^n as a seq_t. */
    seq_t pow5 = ipow((seq_t)5, (seq_t)n);

    /* Multiply by 15 and divide by 62; integer division truncates towards
       zero, which is the required floor for positive values. */
    return (pow5 * (seq_t)15) / (seq_t)62;
}
