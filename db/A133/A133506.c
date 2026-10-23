/*
 * A133506  Numbers n such that powertrain(n) < n.
 * https://oeis.org/A133506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   powertrain_lt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A133506: numbers n such that powertrain(n) < n
   (powertrain is defined by starting with the most significant digit
   and repeatedly raising to the next digit).
   This implementation tests each candidate until the n‑th term is found. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/powertrain_lt.h"

/* Return the n‑th term of A133506 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, guard against overflow */

    idx_t count = 0;               /* how many qualifying numbers found */
    idx_t candidate = 1;           /* start checking from 1 upward */

    while (count < n) {
        ++candidate;
        if (powertrain_lt(candidate))
            ++count;
    }

    return (seq_t)candidate;
}
