/*
 * A170306  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 (n = 0) a(n) = 9 * 8^(n-1) (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of reduced words of length n in the Coxeter group described. */

#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)9;
    /* Multiply by 8 for each subsequent letter; no double count allowed. */
    for (idx_t i = 1; i < n; ++i)
        result *= (seq_t)8;

    return result;
}
