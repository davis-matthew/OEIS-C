/*
 * A156362  a(2*n+2) = 8*a(2*n+1), a(2*n+1) = 8*a(2*n) - 7^n*A000108(n),
 *          a(0)=1.
 * https://oeis.org/A156362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2*n+2) = 8*a(2*n+1); a(2*n+1) = 8*a(2*n) - 7^n * Catalan(n);
 *            a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t a_prev = (seq_t)1;   /* a[0] */
    seq_t pow7   = (seq_t)1;   /* 7^k, k starts at 0 */
    seq_t catal  = (seq_t)1;   /* Catalan(0) */

    for (idx_t k = 0; ; ++k) {
        /* a(2*k + 1) */
        seq_t term = pow7 * catal;            /* 7^k * Catalan(k) */
        seq_t a_next = ((seq_t)8 * a_prev) - term;
        idx_t idx1 = 2*k + 1;
        if (idx1 == n)
            return a_next;

        /* a(2*k + 2) */
        seq_t a_next_next = ((seq_t)8 * a_next);
        idx_t idx2 = 2*k + 2;
        if (idx2 == n)
            return a_next_next;

        /* prepare for next iteration */
        a_prev = a_next_next;

        /* update 7^k */
        pow7 = pow7 * (seq_t)7;

        /* update Catalan number to Catalan(k+1) */
        /* Catalan(k+1) = Catalan(k) * 2*(2*k+1) / (k+2) */
        idx_t numer_i = 2 * (2 * k + 1);
        seq_t numer   = (seq_t)numer_i;
        seq_t denom   = (seq_t)(k + 2);
        catal = (catal * numer) / denom;
    }
}
