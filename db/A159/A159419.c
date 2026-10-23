/*
 * A159419  Number of n X n arrays of squares of integers summing to 27.
 * https://oeis.org/A159419
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^27 in (1 + x + x^4 + x^9 + x^16 +
 *            x^25)^{n^2}
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating counts of each square value. */

#include <stdint.h>
#include <stdbool.h>

/* multiplicative binomial coefficient: binom_seq(N,k) = N!/(k! (N-k)!) */
static seq_t binom_seq(idx_t N, idx_t k)
{
    if (k > N) return 0;
    if (k > N - k) k = N - k;          /* symmetry */
    if (k == 0) return 1;
    seq_t result = 1;
    /* result = product_{i=1..k} (N - k + i) / i  */
    for (idx_t i = 1; i <= k; ++i) {
        result = result * (N - k + i) / i;
    }
    return result;
}

/* required function */
seq_t a(idx_t n)
{
    if (n < 2) return 0;
    idx_t N = n * n;           /* total number of cells (fits in idx_t) */
    seq_t total = 0;

    /* Iterate over all solutions to 25*c5 + 16*c4 + 9*c3 + 4*c2 + c1 = 27 */
    for (idx_t c5 = 0; c5 <= 27 / 25 && c5 <= N; ++c5) {
        idx_t rem5 = 27 - 25 * c5;
        for (idx_t c4 = 0; c4 <= rem5 / 16 && c4 <= N; ++c4) {
            idx_t rem4 = rem5 - 16 * c4;
            for (idx_t c3 = 0; c3 <= rem4 / 9 && c3 <= N; ++c3) {
                idx_t rem3 = rem4 - 9 * c3;
                for (idx_t c2 = 0; c2 <= rem3 / 4 && c2 <= N; ++c2) {
                    idx_t rem2 = rem3 - 4 * c2;
                    idx_t c1 = rem2;            /* remaining must be 1's */
                    if (c1 > 27) continue;
                    idx_t used = c1 + c2 + c3 + c4 + c5;
                    if (used > N) continue;      /* insufficient cells */

                    /* multinomial coefficient for this combination */
                    seq_t term = binom_seq(N, c1);
                    idx_t remaining = N - c1;
                    if (c2 > 0) term = term * binom_seq(remaining, c2);
                    remaining -= c2;
                    if (c3 > 0) term = term * binom_seq(remaining, c3);
                    remaining -= c3;
                    if (c4 > 0) term = term * binom_seq(remaining, c4);
                    remaining -= c4;
                    if (c5 > 0) term = term * binom_seq(remaining, c5);

                    total = total + term;
                }
            }
        }
    }
    return total;
}
