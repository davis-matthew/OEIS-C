/*
 * A001855  Sorting numbers: maximal number of comparisons for sorting n
 *          elements by binary insertion.
 * https://oeis.org/A001855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*ceil(log_2(n)) - 2^ceil(log_2(n)) + 1
 * Range:     u8 n <= 53, u16 n <= 5671, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ceildiv2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ceildiv2.h"

seq_t a(idx_t n) {
  int k = ceildiv2(n);
  seq_t pow2k = (seq_t)1 << k;
  return (seq_t)n * (seq_t)k - pow2k + 1;
}
