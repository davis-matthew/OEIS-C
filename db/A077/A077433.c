/*
 * A077433  Number of separating zeros to represent A077431.
 * https://oeis.org/A077433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor((floor(log10(2*n^2))+1)/2)
 * Range:     u8 n <= 15, u16 n <= 777, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log10_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log10_seq.h"

seq_t a(idx_t n)
{
    /* compute 2 * n^2 */
    seq_t t = (seq_t)n;
    t = t * t;          /* n^2 */
    t = t * (seq_t)2;   /* 2*n^2 */

    /* number of decimal digits of 2*n^2 */
    seq_t digits = floor_log10_seq(t) + 1;

    /* floor of half the digit count */
    return digits / 2;
}
