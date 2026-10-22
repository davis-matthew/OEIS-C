/*
 * A007930  Some nontrivial permutation of digits gives an even number.
 * https://oeis.org/A007930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search: a(n) is the n-th number that is even with at least 3
 *            digits, or that has an even digit somewhere other than the last
 *            position
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
  idx_t count = 0;
  seq_t candidate = 0;

  while (count < n) {
    candidate++;
    bool ok = false;
    if (candidate >= 100 && (candidate % 2) == 0) {
      ok = true;
    } else {
      seq_t num = candidate / 10;
      while (num > 0) {
        if (((num % 10) % 2) == 0) {
          ok = true;
          break;
        }
        num /= 10;
      }
    }
    if (ok) count++;
  }

  return candidate;
}
