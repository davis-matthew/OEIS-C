/*
 * A129530  a(n) = (1/2)*n*(n-1)*3^(n-1).
 * https://oeis.org/A129530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*n*(n-1)*3^(n-1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 35, u128 n <= 74,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

/* Compute the OEIS sequence A129530:
 * a(n) = (1/2)*n*(n-1)*3^(n-1), with a(0)=0, a(1)=0, …
 * Uses the provided pow3(n) helper for 3^n. */

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    seq_t coeff = ((seq_t)n * (seq_t)(n - 1)) / 2;
    seq_t p3 = pow3(n - 1);
    return coeff * p3;
}
