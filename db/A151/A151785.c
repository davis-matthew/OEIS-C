/*
 * A151785  a(n) = 7^(wt(n) - 1) where wt(n) is the binary weight of n
 *          (A000120).
 * https://oeis.org/A151785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7^(wt(n)-1) where wt(n) is the binary weight (Hamming
 *            weight) of n
 * Range:     u8 n <= 14, u16 n <= 126, u32 n <= 8190, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; guard against n == 0 for safety. */
    if (n == 0) return (seq_t)1;

    /* Compute binary weight of n (number of set 1‑bits). */
    idx_t m = n;
    int weight = 0;
    while (m != 0) {
        m &= m - 1;    /* clear the lowest set bit */
        weight++;
    }

    /* Exponent is weight - 1. Since n >= 1, weight >= 1, so exp >= 0. */
    idx_t exp = (idx_t)(weight - 1);

    /* Return 7^exp in the appropriate seq_t type. */
    return seq_pow((seq_t)7, exp);
}
