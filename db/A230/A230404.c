/*
 * A230404  a(n) = the largest k such that (k+1)! divides 2n; the number of
 *          trailing zeros in the factorial base representation of even
 *          numbers.
 * https://oeis.org/A230404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the largest k such that (k+1)! divides 2n
 * Range:     u8 n <= 524, u16 n <= 9294, u32 n <= 9294, u64 n <= 9294,
 *            u128 n <= 9294, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t m = (seq_t)n * (seq_t)2;
    seq_t fact = (seq_t)1;   /* (k+1)! for current k */
    seq_t best = (seq_t)0;
    for (idx_t k = 0; ; ++k)
    {
        if (m % fact == 0) best = (seq_t)k;
        seq_t mul = (seq_t)(k + 2);   /* multiply by (k+2) to get next factorial */
        if (fact > m / mul) break;   /* next factorial would exceed m */
        fact = fact * mul;
    }
    return best;
}
