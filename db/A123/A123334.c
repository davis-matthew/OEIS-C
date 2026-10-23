/*
 * A123334  a(n) = 4^n*(Gamma(n+1/4)/Gamma(1/4) + (n-1)!).
 * https://oeis.org/A123334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0}^{n-1}(4k+1) + 4^n · (n‑1)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(n) = A007696(n) + A000302(n)*A000142(n‑1) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* undefined for n < 1, safe guard */

    /* product of (4k+1) for k = 0 .. n-1    */
    seq_t prod = (seq_t)1;
    for (idx_t k = (idx_t)0; k < n; ++k)
        prod *= (seq_t)(4 * k + 1);

    /* 4^n */
    seq_t pow4 = (seq_t)1;
    for (idx_t k = (idx_t)0; k < n; ++k)
        pow4 *= (seq_t)4;

    /* (n-1)!  (empty product = 1 when n=1) */
    seq_t fact = (seq_t)1;
    for (idx_t k = (idx_t)2; k <= n - 1; ++k)
        fact *= (seq_t)k;

    return prod + pow4 * fact;
}
