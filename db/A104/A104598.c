/*
 * A104598  Expansion of (1-z-sqrt(1-4z))/(1-4z)^2.
 * https://oeis.org/A104598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*n-2)*(3*n+4) - (n+1)*C(2*n+1,n) Initial terms:
 *            a(1)=1, a(2)=10 (Sequence A104598: Expansion of
 *            (1-z-sqrt(1-4z))/(1-4z)^2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, return 0 as safe guard */
    if (n == 1) return (seq_t)1;  /* a(1) = 1 */
    if (n == 2) return (seq_t)10; /* a(2) = 10 */

    /* Compute 2^(2*n-2) by repeated multiplication */
    idx_t exp = 2 * n - 2;
    seq_t pow = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        pow *= (seq_t)2;

    /* Compute term = 2^(2*n-2) * (3*n + 4) */
    seq_t term = pow * ((seq_t)3 * n + (seq_t)4);

    /* Compute binomial coefficient C(2*n+1, n) */
    seq_t bin = binom(2 * n + 1, n);

    /* Subtract (n+1) * bin from term */
    term -= ((seq_t)(n + 1)) * bin;

    return term;
}
