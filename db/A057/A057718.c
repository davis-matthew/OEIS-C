/*
 * A057718  A036917/8 (omitting leading term of A036917).
 * https://oeis.org/A057718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/8) * sum_{k=0}^{n} binomial(2k,k)^2 *
 *            binomial(2n-2k,n-k)^2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Allocate array for C(k) = binomial(2k,k) */
    seq_t *C = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!C) return 0;               /* allocation failure -> zero */

    /* C[0] = 1. Compute C(k) recursively:
       C(k) = C(k-1) * (4k-2) / k */
    seq_t bk = 1;
    C[0] = 1;
    for (idx_t k = 1; k <= n; ++k) {
        bk = (bk * (seq_t)(4 * k - 2)) / k;
        C[k] = bk;
    }

    /* Accumulate sum in a way that avoids overflow by dividing by 8
       after every term.  We keep two parts:
       high   : sum of (term >> 3) plus carries from the low part
       rem    : current remainder modulo 8, always < 8 */
    seq_t high = 0;
    seq_t rem  = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t prod = C[k] * C[n - k];              /* product of two binomials */
        seq_t term = prod * prod;                  /* product squared (C(k)^2 * C(n-k)^2) */

        seq_t term_high = term >> 3;                /* term / 8 */
        seq_t term_low  = term & 7;                /* term % 8 */

        rem += term_low;
        if (rem >= 8) {
            seq_t add = rem >> 3;                  /* floor(rem/8) */
            rem &= 7;                              /* rem % 8 */
            high += add;
        }

        high += term_high;
    }

    /* Final result: divide remaining rem by 8 (must be zero because sum is divisible by 8) */
    seq_t result = high + (rem >> 3);

    free(C);
    return result;
}
