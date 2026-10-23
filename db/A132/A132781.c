/*
 * A132781  Numbers not containing '1010' in binary representation.
 * https://oeis.org/A132781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   the numbers whose binary representation does not contain the
 *            substring "1010". The n-th such number (offset 1) is returned
 * Range:     u8 n <= 188, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_1010
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_1010.h"

/* Return the n-th term (1‑based) of sequence A132781. */
seq_t a(idx_t n)
{
    idx_t seen = 0;     /* how many valid numbers have been found */
    seq_t x = 0;        /* candidate integer */

    while (true) {
        if (!has_1010(x)) {
            ++seen;
            if (seen == n)
                return x;
        }
        ++x;
    }
}
