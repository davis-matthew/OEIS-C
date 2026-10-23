/*
 * A178295  Number of collinear point 4-tuples in a 5 X 5 X 5 X...
 *          n-dimensional cubic grid.
 * https://oeis.org/A178295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9^n/2 + 3*7^n/2 - 2*5^n. Recurrence: a(n) = 21*a(n-1) -
 *            143*a(n-2) + 315*a(n-3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* compute the required powers */
    seq_t pow9 = big_pow((seq_t)9, (seq_t)n);
    seq_t pow7 = big_pow((seq_t)7, (seq_t)n);
    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);

    /* sum = pow9 + 3*pow7 .   Use only addition to avoid signed subtractions */
    seq_t sum = pow9;
    sum += pow7;
    sum += pow7;
    sum += pow7;          /* now sum = 9^n + 3*7^n */

    /* divide by two */
    seq_t half = sum / ((seq_t)2);

    /* twice5 = 2*5^n */
    seq_t twice5 = pow5;
    twice5 += pow5;

    /* final result: half - twice5 */
    return half - twice5;
}
