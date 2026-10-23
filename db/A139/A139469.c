/*
 * A139469  a(n) = Sum_{k=0..n} C(n,3k+2)^2.
 * https://oeis.org/A139469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊(n-2)/3⌋} binomial(n,3k+2)^2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* r = 2,5,8,… ≤ n */
    for (idx_t r = 2; r <= n; r += 3) {
        seq_t c = binomial(n, r);
        sum += c * c;          /* add binomial(n,r)^2 */
    }
    return sum;
}
