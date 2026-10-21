/*
 * A085785  Start at (2n+1)/4 and iterate the map x -> x*ceiling(x); sequence
 *          gives number of steps for denominator to drop to 1 or 2; or -1 if
 *          this never happens.
 * https://oeis.org/A085785
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of x -> x*ceil(x) starting from
 *            (2n+1)/4 until the denominator of the reduced fraction becomes 1
 *            or 2. The sequence is defined for n ≥ 2
 * Range:     u8 n <= 4, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2)                       /* undefined for n < 2, keep return 0 */
        return (seq_t)0;

    /* initial numerator (2n+1); denominator is fixed at 4 */
    seq_t p = ((seq_t)n * (seq_t)2) + (seq_t)1;
    seq_t steps = (seq_t)0;

    while (true) {
        /* ceil(p/4) = (p + 3) / 4 */
        seq_t k = (p + (seq_t)3) / (seq_t)4;
        steps++;

        /* if k is even, denominator will drop to 1 or 2 – stop */
        if ((k & (seq_t)1) == (seq_t)0)
            break;

        /* update numerator for next iteration */
        p = p * k;
        /* loop guard to avoid infinite loops in pathological cases */
        if (steps > (seq_t)1000)       /* very unlikely for any n */
            return (seq_t)-1;
    }
    return steps;
}
