/*
 * A101257  Remainder when the least divisor of n greater than or equal to the
 *          square root of n (A033677(n)) is divided by the greatest divisor
 *          of n less than or equal to the square root of n (A033676(n)).
 * https://oeis.org/A101257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A033677(n) mod A033676(n)
 * Range:     u8 n <= 3302, u16 n <= 9623, u32 n <= 9623, u64 n <= 9623,
 *            u128 n <= 9623, big any n
 * Helpers:   integer_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for n ≥ 1 where A033677(n) = smallest divisor ≥ √n
 * and A033676(n) = largest divisor ≤ √n.
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/integer_sqrt.h"

seq_t a(idx_t n)
{
    /* Handle n = 0 explicitly – not part of the sequence but avoids
       undefined behaviour on some callers. */
    if (n == 0)
        return 0;

    /* Floor of sqrt(n).  integer_sqrt returns an unsigned integer. */
    idx_t s = integer_sqrt(n);

    /* Largest divisor ≤ √n is found by checking the numbers in
       descending order until a divisor is found.  For composite n
       this will be non‑trivial; for prime n the answer is 1. */
    idx_t d2 = 1; /* 1 always divides n */
    for (idx_t i = s; i >= 1; --i)
    {
        if ((n % i) == 0)
        {
            d2 = i;
            break;
        }
    }

    /* Smallest divisor ≥ √n is just n / d2. */
    seq_t d2_seq = (seq_t)d2;
    seq_t d1_seq = (seq_t)(n / d2);

    /* Return the remainder of the quotient by the divisor. */
    return d1_seq % d2_seq;
}
