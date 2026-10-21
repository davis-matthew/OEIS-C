/*
 * A069558  Triangular numbers in which the k-th digit, numbering them from
 *          right to left, either divides k or is a nonzero multiple of k. Is
 *          1 only when k has no other single-digit divisor (less than 10).
 * https://oeis.org/A069558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, hard, more, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   tri_is_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A069558 – Triangular numbers whose k‑th digit (from the right)
   either divides k or is a non‑zero multiple of k.  
   The digit 1 is allowed only when k has no divisor between 2 and 9
   (i.e., all prime factors of k are >7).  This implementation
   tests each triangular number sequentially and returns the n‑th
   valid one. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/tri_is_valid.h"

/* Forward declaration – seq_t and idx_t are supplied by the build */
seq_t a(idx_t n);

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 1; ; ++m) {
        /* m‑th triangular number: (m*(m+1))/2 using seq_t arithmetic */
        seq_t tri = ((seq_t)m * ((seq_t)m + 1)) / 2;

        if (tri_is_valid(tri)) {
            ++count;
            if (count == n)
                return tri;
        }
    }
}
