/*
 * A060548  a(n) is the number of D3-symmetric patterns that may be formed
 *          with a top-down equilateral triangular arrangement of closely
 *          packed black and white cells satisfying the local matching rule of
 *          Pascal's triangle modulo 2, where n is the number of cells in each
 *          edge of the arrangement. The matching...
 * https://oeis.org/A060548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = 2*a(n-6) for n ≥ 7; The first six terms are a(1)=2,
 *            a(2)=1, a(3)=2, a(4)=2, a(5)=2, a(6)=2
 * Range:     u8 n <= 42, u16 n <= 90, u32 n <= 186, u64 n <= 378,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The six initial values of the sequence. */
    static const seq_t init[6] = { (seq_t)2, (seq_t)1,
                                   (seq_t)2, (seq_t)2,
                                   (seq_t)2, (seq_t)2 };

    /* Direct return for the first six indices. */
    if (n <= 6)
        return init[n - 1];

    /* Compute how many times we can apply the recurrence a(n)=2*a(n-6). */
    idx_t periods = (n - 1) / 6;          /* floor((n-1)/6)  */
    idx_t remainder = n - 6 * periods;   /* 1 ≤ remainder ≤ 6 */

    /* Start from the base value and double it 'periods' times. */
    seq_t result = init[remainder - 1];
    for (idx_t i = 0; i < periods; ++i)
        result = result * (seq_t)2;

    return result;
}
