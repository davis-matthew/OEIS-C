/*
 * A101862  a(n) = n*(n+1)*(n+7)*(122+57*n+n^2)/120.
 * https://oeis.org/A101862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+7)*(122+57*n+n^2)/120
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   reduce_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reduce_factor.h"

seq_t a(idx_t n)
{
    /* Convert the index to the output type */
    seq_t N   = (seq_t)n;
    seq_t d   = (seq_t)120;   /* Remaining part of the denominator   */
    seq_t res = (seq_t)1;     /* Accumulator for the product         */

    /* Factor n */
    seq_t f = N;
    reduce_factor(&f, &d);
    res *= f;

    /* Factor n+1 */
    f = N + (seq_t)1;
    reduce_factor(&f, &d);
    res *= f;

    /* Factor n+7 */
    f = N + (seq_t)7;
    reduce_factor(&f, &d);
    res *= f;

    /* Factor n^2 + 57*n + 122 */
    f = N * N + (seq_t)57 * N + (seq_t)122;
    reduce_factor(&f, &d);
    res *= f;

    /* Any remaining part of the denominator (should be 1) */
    if (d > (seq_t)1) {
        res /= d;
    }

    return res;
}
