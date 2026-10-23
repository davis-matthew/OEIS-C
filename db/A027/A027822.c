/*
 * A027822  a(n) = 42*(n+1)*binomial(n+6,10).
 * https://oeis.org/A027822
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 42*(n+1)*binomial(n+6,10)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 19, u64 n <= 157, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For n < 4 the sequence is not defined; return 0 */
    if (n < 4)
        return 0;

    /* Compute binomial(n+6, 10) safely */
    seq_t b = binomial(n + 6, 10);

    /* Multiply by (n+1) and then by 42 */
    b = b * (seq_t)(n + 1);
    b = b * 42;

    return b;
}
