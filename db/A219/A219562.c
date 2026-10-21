/*
 * A219562  a(n) = Sum_{k=0..n} binomial(n+k,k)^4.
 * https://oeis.org/A219562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+k,k)^4
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t c   = 1;                 /* binomial(n+0,0) = 1 */

    for (idx_t k = 0; k <= n; ++k) {
        /* add (C(n+k,k))^4 to the sum */
        seq_t c2 = c * c;
        seq_t c4 = c2 * c2;
        sum += c4;

        /* Prepare binomial(n+k+1, k+1) for next iteration */
        if (k < n) {
            seq_t num = (seq_t)(n + k + 1);  /* numerator (n + k + 1)  */
            seq_t den = (seq_t)(k + 1);      /* denominator (k + 1)    */
            c = (c * num) / den;            /* exact division */
        }
    }
    return sum;
}
