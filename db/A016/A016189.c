/*
 * A016189  a(n) = 10^n - 9^n.
 * https://oeis.org/A016189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n - 9^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Compute 10^n and 9^n using big‑integer exponentiation. */
    seq_t p10 = seq_pow((seq_t)10, n);
    seq_t p9  = seq_pow((seq_t)9 , n);
    /* a(n) = 10^n - 9^n */
    return p10 - p9;
}
