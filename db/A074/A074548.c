/*
 * A074548  a(n) = 3^n + 4^n + 6^n.
 * https://oeis.org/A074548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n + 4^n + 6^n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Each term computed as the sum of powers of 3, 4 and 6. */
#include "../utils/big_pow.h"

/* Computes the n‑th term of OEIS sequence A074548.
   n is zero‑based; a(0) = 3. */
seq_t a(idx_t n)
{
    /* Use the generic big_pow routine which handles both small and
       arbitrary‑precision seq_t types efficiently. */
    seq_t p3 = big_pow((seq_t)3, (seq_t)n);
    seq_t p4 = big_pow((seq_t)4, (seq_t)n);
    seq_t p6 = big_pow((seq_t)6, (seq_t)n);
    return p3 + p4 + p6;
}
