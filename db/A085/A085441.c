/*
 * A085441  a(n) = Sum_{i=1..n} binomial(i+1,2)^6.
 * https://oeis.org/A085441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=1..n} binomial(i+1,2)^6
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 49, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        /* Compute binomial(i+1,2) = i*(i+1)/2 */
        seq_t bi = ((seq_t)i * ((seq_t)i + 1)) / 2;
        /* Compute bi^6 by repeated multiplication */
        seq_t bi2 = bi * bi;      /* bi^2 */
        seq_t bi3 = bi2 * bi;     /* bi^3 */
        seq_t bi4 = bi3 * bi;     /* bi^4 */
        seq_t bi5 = bi4 * bi;     /* bi^5 */
        seq_t bi6 = bi5 * bi;     /* bi^6 */
        sum += bi6;
    }
    return sum;
}
