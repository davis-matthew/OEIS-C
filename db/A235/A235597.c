/*
 * A235597  Squares in which each digit exceeds the previous digit by more
 *          than one.
 * https://oeis.org/A235597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   is_good_sq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A235597: Squares in which each digit exceeds the previous digit by more than one. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_good_sq.h"

/* Return the n-th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    static seq_t terms[20];
    static size_t count = 0;

    if (count == 0) {
        /* Precompute all terms.  
           All qualifying squares are less than 100000 (=10^5). */
        const seq_t LIMIT = 100000 - 1;     /* largest 5‑digit number */
        for (seq_t m = 0;; ++m) {
            seq_t sq = (seq_t)m * (seq_t)m;
            if (sq > LIMIT)
                break;
            if (is_good_sq(sq))
                terms[count++] = sq;
        }
    }

    if (n == 0 || (size_t)(n - 1) >= count)
        return (seq_t)0;                  /* out of bounds → 0 */

    return terms[n - 1];
}
