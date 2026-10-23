/*
 * A168914  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^21 = I.
 * https://oeis.org/A168914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   for n = 0: a(0) = 1 for n >= 1: a(n) = 45 * 44^(n-1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   seq_mul, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This counts words of length n over 45 generators
   with no consecutive equal generators. */

#include <stdint.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return seq_one();
    }
    seq_t pow44 = seq_pow((seq_t)44, n-1);
    return seq_mul((seq_t)45, pow44);
}
