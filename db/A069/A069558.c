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
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-01)
 * Helpers:   none
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

/* Helper: test whether a triangular number satisfies the digit rules */
static bool tri_is_valid(seq_t tri)
{
    seq_t temp = tri;
    idx_t position = 1;                 /* 1‑based from the rightmost digit */

    while (temp > 0) {
        uint8_t digit = (uint8_t)(temp % 10);   /* extract least‑significant digit */
        temp = temp / 10;                      /* shift right */

        if (digit == 0)
            return false;                      /* zero digits are disallowed */

        if (digit == 1) {
            /* digit 1 is allowed only if position has no divisor 2…9 */
            if (position > 1) {
                if ((position % 2 == 0) ||
                    (position % 3 == 0) ||
                    (position % 5 == 0) ||
                    (position % 7 == 0))
                    return false;
            }
        } else {
            /* for other digits: position divides digit or digit is a multiple of position */
            if ((position % (idx_t)digit != 0) &&
                (((idx_t)digit) % position != 0))
                return false;
        }

        position++;
    }
    return true;
}

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
