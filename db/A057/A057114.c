/*
 * A057114  Permutation of N induced by the order-preserving permutation of
 *          the rational numbers (x -> x+1); positions in Stern-Brocot tree.
 * https://oeis.org/A057114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of ( fraction at position n ) + 1 in the full
 *            (extended) Stern–Brocot tree
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   fraction_at_index, index_of_fraction, msb_pos
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fraction_at_index.h"
#include "../utils/index_of_fraction.h"
#include "../utils/msb_pos.h"

/* Main function: a(idx_t n) as required by the problem statement.   */
seq_t a(idx_t n)
{
    long long num, den;
    fraction_at_index(n, &num, &den);

    /* fraction + 1 */
    long long target_num = num + den;  /* numerator after adding 1 */
    long long target_den = den;        /* denominator unchanged */

    idx_t idx = index_of_fraction(target_num, target_den);
    return (seq_t)idx;
}
