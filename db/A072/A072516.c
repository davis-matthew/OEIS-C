/*
 * A072516  Sum of remainders when n is divided by lesser squares.
 * https://oeis.org/A072516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor(sqrt(n))} (n mod k^2)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   int_sqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/int_sqrt_seq.h"
/* The function uses a simple binary‑search integer square root
   that works for any unsigned seq_t. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;
    seq_t max_k = int_sqrt_seq((seq_t)n);

    for (idx_t k = 1; k <= (idx_t)max_k; ++k)
    {
        seq_t square = (seq_t)k * (seq_t)k;          /* k^2 */
        seq_t rem = ((seq_t)n) % square;           /* n mod k^2 */
        sum += rem;
    }
    return sum;
}
