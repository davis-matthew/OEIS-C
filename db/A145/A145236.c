/*
 * A145236  a(n) is the least positive integer such that if p_n is the n-th
 *          prime then (ceiling(sqrt(a(n)*p_n)))^2 - a(n)*p_n is a perfect
 *          square.
 * https://oeis.org/A145236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_n - 2*floor(sqrt(2*p_n)) + 2 for n>1, a(1)=2
 * Range:     u8 n <= 11, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* the first term is given explicitly */
    if (n <= 1)
        return (seq_t)2;

    /* nth prime */
    seq_t pn = (seq_t)get_prime(n);

    /* compute floor( sqrt( 2 * p_n ) ) */
    seq_t two = (seq_t)2;
    seq_t sq = floor_sqrt(pn * two);

    /* closed‑form expression from the OEIS entry */
    return pn - two * sq + two;
}
