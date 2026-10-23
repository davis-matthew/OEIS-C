/*
 * A010806  18th powers: a(n) = n^18.
 * https://oeis.org/A010806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^18
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 11, u128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the nth 18th power.
   For any seq_t (built‑in unsigned, 128‑bit, or arbitrary precision) it
   performs binary exponentiation using only integer arithmetic. */
seq_t a(idx_t n)
{
    seq_t base = (seq_t)n;
    seq_t result = 1;
    idx_t exp = 18;          /* exponent is fixed at 18 */

    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}
