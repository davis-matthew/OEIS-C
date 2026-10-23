/*
 * A117142  Number of partitions of n in which any two parts differ by at most
 *          2.
 * https://oeis.org/A117142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^2 + 10*n + 3 + (-1)^n * (2*n - 3))/16
 * Range:     u8 n <= 9, u16 n <= 261, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert to seq_t for computation                                    */
    seq_t nn = (seq_t)n;

    /* term = (-1)^n * (2*n - 3)                                          */
    seq_t term = (n & 1ULL) ? -(2 * nn - 3) : (2 * nn - 3);

    /* numerator = 2*n^2 + 10*n + 3 + term                               */
    seq_t numerator = 2 * nn * nn + 10 * nn + 3 + term;

    /* division by 16 gives integer result                               */
    return numerator / (seq_t)16;
}
