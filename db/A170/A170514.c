/*
 * A170514  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170514
   Formula: a(0) = 1
            a(n) = 25 * 24^(n-1)   for n ≥ 1
   This is the number of reduced words of length n in the Coxeter group with
   25 generators S_i satisfying S_i^2 = I and (S_i S_j)^46 = I for i ≠ j.
   The sequence grows as 1, 25, 600, 14400, 345600, …, 25·24^(n-1). */

#include <stdint.h>

seq_t a(idx_t n) {
    /* a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* a(n) = 25 * 24^(n-1) for n ≥ 1 */
    seq_t result = (seq_t)25;
    for (idx_t i = 1; i < n; ++i) {
        result *= (seq_t)24;
    }
    return result;
}
