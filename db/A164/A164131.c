/*
 * A164131  Numbers k such that k^2 == 2 (mod 31).
 * https://oeis.org/A164131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (31 + (-1)^(n-1) + 62*(n-1)) / 4
 * Range:     u8 n <= 4, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <assert.h>

seq_t a(idx_t n)
{
    /* The sequence starts at offset 1.  Behaviour for n<1 is
       undefined; callers normally pass n >= 1. */
    assert(n >= 1);

    /* Compute n-1 in seq_t for the algebraic part. */
    seq_t n1 = (seq_t)(n - 1);

    /* Term before division: 31 + 62*n1, with ±1 depending on parity. */
    seq_t term = 31 + 62 * n1;

    /* Add +1 for odd n, subtract 1 for even n. */
    if (n & 1u) {          /* n is odd → (-1)^(n-1) = +1 */
        term += 1;
    } else {               /* n is even → (-1)^(n-1) = -1 */
        /* No overflow: term >= 31, subtracting 1 stays non‑negative. */
        term -= 1;
    }

    /* Final division by 4 gives the desired term. */
    return term / 4;
}
