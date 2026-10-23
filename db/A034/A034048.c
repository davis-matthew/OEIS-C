/*
 * A034048  Numbers with multiplicative digital root value 0.
 * https://oeis.org/A034048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th integer (starting from 0) whose * multiplicative
 *            digital root is 0. The algorithm scans consecutive * integers
 *            and tests each for that property
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_mdr_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_mdr_zero.h"

/* Return the n‑th term of A034048 (1‑based index). */
seq_t a(idx_t n)
{
    idx_t found = 0;            /* number of qualifying integers seen */
    idx_t x = 0;                /* current integer being tested */

    while (found < n) {
        if (has_mdr_zero(x)) {
            ++found;
            if (found == n)
                return (seq_t)x;   /* cast to the output type                      */
        }
        ++x;
    }
    /* Unreachable for n >= 1, but return 0 to satisfy the compiler. */
    return (seq_t)0;
}
