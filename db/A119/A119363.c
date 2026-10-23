/*
 * A119363  a(n) = Sum_{k=0..n} C(n,3k)^2.
 * https://oeis.org/A119363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} C(n,3k)^2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   binom_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_n_k.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; 3 * k <= n; ++k) {
        idx_t m = 3 * k;
        seq_t c = binom_n_k(n, m);   /* C(n, m) */
        sum += c * c;                /* c^2 */
    }
    return sum;
}
