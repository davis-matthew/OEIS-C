/*
 * A049070  a(n) = (n+1)^2*binomial(2*n+2,n-1)/2.
 * https://oeis.org/A049070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)^2 * binomial(2*n+2, n-1) / 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the utility function binom(a,b) defined in ../utils/binom.h */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* By definition a(0) = 0; the binomial would be undefined for n=0 */
    if (n == 0)
        return 0;

    /* Compute binomial(2*n+2, n-1) */
    idx_t upper = 2 * n + 2;
    idx_t lower = n - 1;
    seq_t bin = binom(upper, lower);

    /* (n+1)^2 * bin / 2 */
    seq_t t = (seq_t)(n + 1);
    t = t * bin;              /* multiply by binomial */
    t = t * (seq_t)(n + 1);   /* multiply by (n+1) again */
    t = t / 2;                /* divide by 2 */

    return t;
}
