/*
 * A039480  Numbers whose base-11 representation has the same nonzero number
 *          of 5's and 8's.
 * https://oeis.org/A039480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   has_eq5e8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers whose base-11 representation has the same non-zero
   number of 5's and 8's (OEIS A039480). */
#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_eq5e8.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;  /* offset starts at 1, return 0 for n=0 */
    seq_t count   = 0;
    seq_t candidate = 1;
    while (count < n) {
        if (has_eq5e8(candidate)) {
            count++;
            if (count == n) return candidate;
        }
        candidate++;
    }
    return (seq_t)0; /* unreachable */
}
