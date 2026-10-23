/*
 * A170656  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 23, a(n) = 22 * a(n-1) for n ≥ 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   seq_mul, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalently: a(n) = 23 * 22^(n-1)  for n ≥ 1. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return seq_one();
    }

    /* Compute 22^(n-1) and multiply by 23. */
    seq_t pow = seq_pow((seq_t)22, n - 1);
    return seq_mul(pow, (seq_t)23);
}
