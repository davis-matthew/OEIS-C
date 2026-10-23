/*
 * A185595  a(n) = floor(n^(3/2)) - floor(n^(1/2)); complement of A185596.
 * https://oeis.org/A185595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^(3/2)) - floor(n^(1/2))
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 345, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    if (n == 0) {                 // offset is 1, behavior for n=0 is irrelevant
        return (seq_t)0;
    }
    seq_t ns = (seq_t)n;          // n converted once to seq_t
    seq_t sqrt_n   = floor_sqrt(ns);   // floor(sqrt(n))

    seq_t n_sq = ns * ns;         // n^2
    seq_t n_cu = n_sq * ns;       // n^3

    seq_t sqrt_n3 = floor_sqrt(n_cu);   // floor(sqrt(n^3)) == floor(n^(3/2))

    return sqrt_n3 - sqrt_n;      // final result
}
