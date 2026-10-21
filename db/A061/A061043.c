/*
 * A061043  Numerator of 1/25 - 1/n^2.
 * https://oeis.org/A061043
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy, frac, nice
 * Formula:   a(n) = (n^2-25) / gcd(n^2-25, 25*n^2)
 * Range:     u8 n <= 5, u16 n <= 51, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the numerator of 1/25 - 1/n^2 after reducing to lowest terms.   */
/*  The recursion is implicit via the gcd computation.                 */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Expect n >= 5 – the sequence is defined from that offset.    */
    if (n < 5) {
        /* For n < 5 the problem statement does not require a value,   */
        /* but we return 0 for completeness.                           */
        return 0;
    }

    seq_t sn = (seq_t)n;                    /* convert index to seq_t   */
    seq_t num = sn * sn - (seq_t)25;        /* numerator: n^2 - 25      */
    if (num == 0)                           /* immediate zero case      */
        return (seq_t)0;

    seq_t den = (seq_t)25 * sn * sn;        /* denominator: 25*n^2     */
    seq_t g   = gcd(num, den);              /* common factor           */
    return num / g;                         /* reduced numerator       */
}
