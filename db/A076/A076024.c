/*
 * A076024  a(n) = (2^n + 4)*(2^n - 1)/6.
 * https://oeis.org/A076024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^n + 4)*(2^n - 1)/6 Computation avoids overflow by
 *            dividing by 2 and 3 before the multiplication
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t t = seq_pow((seq_t)2, n);   /* t = 2^n */
    seq_t A = t + (seq_t)4;
    seq_t B = t - (seq_t)1;

    if ((n & 1) == 0) {                /* n even: B divisible by 3, A by 2 */
        seq_t A2 = A >> 1;            /* A/2 */
        seq_t B3 = B / (seq_t)3;       /* B/3 */
        return A2 * B3;                /* (A/2)*(B/3) = a(n) */
    } else {                          /* n odd: A divisible by 6 */
        seq_t A6 = A / (seq_t)6;       /* A/6 */
        return A6 * B;                 /* (A/6)*B = a(n) */
    }
}
