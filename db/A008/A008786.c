/*
 * A008786  a(n) = (n+5)^n.
 * https://oeis.org/A008786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+5)^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: compute base^exp using binary exponentiation.
 * Works for any seq_t (unsigned integer or arbitrary‑precision class). */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Sequence A008786: a(n) = (n+5)^n,  n ≥ 0. */
seq_t a(idx_t n)
{
    seq_t base = (seq_t)n + 5;
    return pow_seq(base, n);
}
