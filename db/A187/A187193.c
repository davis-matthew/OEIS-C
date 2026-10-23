/*
 * A187193  Number of 6-turn rook's tours on an n X n board summed over all
 *          starting positions.
 * https://oeis.org/A187193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 96*( 44*binom(n,7) + 503*binom(n+1,7) + 812*binom(n+2,7)
 *            + 306*binom(n+3,7) + 15*binom(n+4,7) );
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

/* Return the n-th term of sequence A187193 (offset 1). */
seq_t a(idx_t n)
{
    /* For n = 1 or 2 the result is zero. The formula below also
       gives zero, but this early return avoids unneeded binom calls.   */
    if (n < 3)
        return 0;

    seq_t sum = 0;
    seq_t t;

    t = binom(n, 7);
    sum += (seq_t)44 * t;

    t = binom(n + 1, 7);
    sum += (seq_t)503 * t;

    t = binom(n + 2, 7);
    sum += (seq_t)812 * t;

    t = binom(n + 3, 7);
    sum += (seq_t)306 * t;

    t = binom(n + 4, 7);
    sum += (seq_t)15 * t;

    return (seq_t)96 * sum;
}
