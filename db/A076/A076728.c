/*
 * A076728  a(n) = (n-1)^2 * n^(n-2).
 * https://oeis.org/A076728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)^2 * n^(n-2) (offset 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for uint64_t, may be unused but harmless */

/* Compute base^exp for unsigned integer exponent exp.
   Uses binary exponentiation; works for arbitrary‑precision seq_t. */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t cur = base;
    while (exp > 0) {
        if (exp & (idx_t)1)
            result *= cur;
        cur *= cur;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Handle the special cases explicitly.       */
    /* For n == 0 (outside the offset) return 0,
       for n == 1 the formula yields 0 anyway.     */
    if (n <= 1)
        return (seq_t)0;

    /* Compute (n-1)^2 */
    seq_t n_minus1 = (seq_t)n - (seq_t)1;
    seq_t square = n_minus1 * n_minus1;

    /* Compute n^(n-2) */
    idx_t exp = n - (idx_t)2;
    seq_t pow_n = pow_seq_t((seq_t)n, exp);

    /* Return the product */
    return square * pow_n;
}
