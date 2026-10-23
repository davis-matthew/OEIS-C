/*
 * A146537  a(n) = A144453(n)/16.
 * https://oeis.org/A146537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numerator(1/9 - 1/(8*n+5)^2) / 16
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 2795, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That equals ( (8*n+5)^2 - 9 ) / gcd( (8*n+5)^2 - 9 , 9*(8*n+5)^2 ) ) / 16 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Compute m = 8*n + 5 */
    seq_t m = (seq_t)n * 8 + 5;

    /* Compute m^2 and numerator and denominator of the difference */
    seq_t m2    = m * m;          /* m^2 */
    seq_t num   = m2 - 9;         /* m^2 - 9   */
    seq_t denom = 9 * m2;         /* 9 * m^2   */

    /* Reduce the fraction (m^2 - 9)/(9*m^2) to lowest terms */
    seq_t g = gcd(num, denom);

    /* The numerator after reduction */
    seq_t reduced = num / g;

    /* Divide by 16 according to the sequence definition */
    return reduced / 16;
}
