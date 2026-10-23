/*
 * A043997  Numbers k such that 0 and 2 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A043997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_adjacent_02_or_20
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: brute‑force search for k such that 0 and 2 occur juxtaposed
 * in the decimal representation of k but not in k+1.
 * a(1) = 20, a(2) = 102, ... as in OEIS A043997. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_adjacent_02_or_20.h"

/* returns the n‑th term of the sequence (offset 1) */
seq_t a(idx_t n)
{
    if (n == 0)  /* the sequence is defined for n ≥ 1; return 0 for safety */
        return (seq_t)0;

    idx_t found = 0;
    uint64_t num  = 1;              /* test integers starting from 1 */

    while (found < n) {
        if (has_adjacent_02_or_20(num) &&
            !has_adjacent_02_or_20(num + 1)) {
            found++;
            if (found == n)
                return (seq_t)num; /* cast to seq_t (assumes a constructor
                                     * or conversion from uint64_t) */
        }
        num++;
    }
    /* unreachable */
    return (seq_t)0;
}
