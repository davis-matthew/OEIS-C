/*
 * A218311  Product of the nonzero digits (in base 10) of n^5.
 * https://oeis.org/A218311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (product of the non‑zero decimal digits of n^5)^2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 17, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Compute n^5. */
    seq_t pow = seq_pow((seq_t)n, 5);

    /* Compute the product of non‑zero digits. */
    seq_t prod = 1;
    while (pow > 0) {
        seq_t digit = pow % 10;
        if (digit != 0)
            prod *= digit;
        pow /= 10;
    }

    /* Return the square of that product. */
    return prod * prod;
}
