/*
 * A073306  a(n) = Product_{2i<n} binomial(2*n-2*i-1, 2*i).
 * https://oeis.org/A073306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=0}^{⌊(n-1)/2⌋} binomial(2*n-2*i-1, 2*i)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case for n = 0 */
    if (n == 0) return 1;

    seq_t result = 1;
    idx_t limit = (n - 1) / 2;           /* floor((n-1)/2) */

    for (idx_t i = 0; i <= limit; ++i) {
        idx_t N = 2 * n - 2 * i - 1;    /* upper argument of the binomial */
        idx_t K = 2 * i;                /* lower argument (0, 2, 4, ...) */

        seq_t bin = 1;
        /* Compute binomial coefficient multiplicatively:
           bin = ∏_{j=1}^{K} (N - K + j) / j   (exact division) */
        for (idx_t j = 1; j <= K; ++j) {
            bin *= N - K + j;
            bin /= j;
        }

        result *= bin;
    }
    return result;
}
