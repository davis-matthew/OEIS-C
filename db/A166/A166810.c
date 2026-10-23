/*
 * A166810  Number of n X 6 1..2 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          nondecreasing order.
 * https://oeis.org/A166810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+6, 6) - 2 (for n >= 1)
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 117, u64 n <= 4862,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial_n_plus_6_choose_6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial_n_plus_6_choose_6.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* Not part of the sequence */
    seq_t val = binomial_n_plus_6_choose_6(n);
    return val - 2;
}
