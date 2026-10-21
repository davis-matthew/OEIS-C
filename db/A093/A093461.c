/*
 * A093461  a(1)=1, a(n) = 2*(n^(n-1)-1)/(n-1) for n >= 2.
 * https://oeis.org/A093461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(n^(n-1)-1)/(n-1) Equivalent form used for computation:
 *            a(n) = 2 * (1 + n + n^2 + … + n^(n-2)) for n ≥ 2 a(1) = 1 The
 *            implementation uses only integer addition and multiplication,
 *            avoiding overflow for all n whose result fits in the built‑in
 *            seq_t type
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1U)
        return (seq_t)1U;

    seq_t sum  = (seq_t)0U;      /* will accumulate 1 + n + … + n^(n-2) */
    seq_t term = (seq_t)1U;      /* current power of n */

    /* add n^0 .. n^(n-2) ; loop runs exactly n-1 times */
    for (idx_t i = (idx_t)0U; i < n - (idx_t)1U; ++i)
    {
        sum  += term;
        term *= (seq_t)n;
    }

    return sum * (seq_t)2U;
}
