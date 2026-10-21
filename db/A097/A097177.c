/*
 * A097177  a(n) = Sum_{k=0..n} binomial(floor((n+1)/2), floor((k+1)/2)) *
 *          10^k.
 * https://oeis.org/A097177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(floor((n+1)/2), floor((k+1)/2)) *
 *            10^k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* m = floor((n+1)/2) */
    idx_t m = (n + 1) >> 1;

    /* Allocate array C[0..m] to hold binomial(m, r) */
    seq_t *C = (seq_t *)malloc((m + 1) * sizeof(seq_t));
    if (!C) return 0;            /* In case of allocation failure */

    /* Initialize C[0] = 1, others 0 */
    C[0] = 1;
    for (idx_t j = 1; j <= m; ++j) C[j] = 0;

    /* Compute binomial row m using Pascal's triangle */
    for (idx_t i = 1; i <= m; ++i) {
        for (idx_t j = i; j > 0; --j) {
            C[j] = C[j] + C[j - 1];
        }
    }

    /* Now compute the sum */
    seq_t result = 0;
    seq_t pow10   = 1;              /* 10^0 */

    for (idx_t k = 0; k <= n; ++k) {
        idx_t r = (k + 1) >> 1;     /* floor((k+1)/2) */
        seq_t term = C[r];
        term = term * pow10;
        result = result + term;
        pow10 = pow10 * 10;
    }

    free(C);
    return result;
}
