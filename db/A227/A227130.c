/*
 * A227130  Numbers k for which there is an even number of nonzero digits when
 *          k is written in the factorial base (A007623).
 * https://oeis.org/A227130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th integer k whose factorial-base representation
 *            contains an even number of non‑zero digits
 * Range:     u8 none, u16 none, u32 n <= 2520, u64 n <= 2520, u128 n <= 2520,
 *            big any n
 * Helpers:   even_nonzero_factorial_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/even_nonzero_factorial_digits.h"

/* Main function required by the problem. */
seq_t a(idx_t n)
{
    /* n is 1‑based.  Enumerate integers k starting from 0 until we
       find the n‑th one with an even number of non‑zero factorial‑base
       digits. */
    idx_t count = 0;
    seq_t k = (seq_t)0;
    while (1) {
        if (even_nonzero_factorial_digits(k)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
