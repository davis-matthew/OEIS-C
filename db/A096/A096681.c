/*
 * A096681  Least k such that decimal representation of k*n contains only
 *          digits 0 and 2.
 * https://oeis.org/A096681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = least k such that decimal representation of k*n *
 *            consists only of digits 0 and 2. * The value returned is k =
 *            (smallest such number)/n
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 98, u64 n <= 998,
 *            u128 n <= 10000, big any n
 * Helpers:   smallest_02_multiple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/smallest_02_multiple.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined, but guard */

    seq_t rep = smallest_02_multiple(n);
    seq_t k = rep / n;               /* exact division */
    return k;
}
