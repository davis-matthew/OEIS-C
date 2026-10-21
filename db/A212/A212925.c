/*
 * A212925  Number of n X 3 0..2 arrays with no column j greater than column
 *          j-1 in all rows.
 * https://oeis.org/A212925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 27^n - 2*9^n + 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute base^exp with exponentiation by squaring.
   Works for any seq_t that supports builtin integer types or
   arbitrary‑precision integer class (compiled as C++17). */
static seq_t
power(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp != 0) {
        if (exp & (idx_t)1) {
            result *= base;
        }
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

/* OEIS A212925: number of n×3 arrays with entries 0..2 that satisfy the
   monotonic column condition (column j ≤ column j‑1 for all rows). */
seq_t a(idx_t n)
{
    /* n is assumed ≥ 1, but return 0 when n is 0 just in case. */
    if (n == 0) {
        return (seq_t)0;
    }

    seq_t pow27 = power((seq_t)27, n);
    seq_t pow9  = power((seq_t)9, n);

    /* a(n) = 27^n - 2*9^n + 1 */
    return pow27 - (seq_t)2 * pow9 + (seq_t)1;
}
