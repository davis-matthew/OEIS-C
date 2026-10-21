/*
 * A050405  Partial sums of A051879.
 * https://oeis.org/A050405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+5,5)*(3*n+2)/2
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 80, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;          /* cast index to seq_t */
    seq_t binom = 1;              /* binomial(n+5,5) */

    /* Compute C(n+5,5) multiplicatively.
       For k = 1..5:  binom = binom * (n + k) / k   */
    for (seq_t k = 1; k <= 5; ++k) {
        binom = binom * (nn + k);
        binom = binom / k;       /* exact division */
    }

    /* Multiply by (3*n + 2) */
    seq_t mul = 3 * nn + 2;
    seq_t result = binom * mul;

    /* Final division by 2 (exact) */
    result = result / 2;

    return result;
}
