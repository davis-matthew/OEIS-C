/*
 * A019550  a(n) is the concatenation of n and 2n.
 * https://oeis.org/A019550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 65, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = concatenation of n and 2*n.
 */
#include "../utils/pow10.h"   /* provides seq_t pow10(idx_t) */

seq_t a(idx_t n)
{
    idx_t two = n + n;

    /* number of decimal digits in 2*n */
    unsigned d = 0;
    idx_t tmp = two;
    while (tmp > 0) {
        tmp /= 10;
        d++;
    }

    /* concat(n, 2*n) = n * 10^d + 2*n */
    seq_t part = (seq_t)n * pow10(d);
    return part + (seq_t)two;
}
