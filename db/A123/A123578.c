/*
 * A123578  The Kruskal-Macaulay function M_2(n).
 * https://oeis.org/A123578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((1 + sqrt(1 + 8*(n-1))) / 2) for n ≥ 1, a(0) = 0.
 *            This is the Kruskal–Macaulay function M₂(n)
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 // base case

    seq_t n_seq = (seq_t)n;               // promote index to seq_t
    seq_t t = (seq_t)8 * (n_seq - 1) + 1; // compute 1 + 8*(n-1)
    seq_t s = floor_sqrt(t);              // integer square root of t
    return (s + 1) / 2;                   // floor((1 + s) / 2)
}
