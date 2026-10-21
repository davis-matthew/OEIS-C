/*
 * A219176  Smallest k > 1 such that k + n divides k^2 + n.
 * https://oeis.org/A219176
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k > 1 such that (k + n) | (k^2 + n)
 * Range:     u8 n <= 15, u16 n <= 291, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (extends to all n ≥ 2; a(n) ≤ n+2) */

#include <stddef.h>  /* for size_t, if needed */

seq_t a(idx_t n)
{
    if (n < 2)                 /* out of range according to OEIS offset */
        return 0; /* undefined for n < 2 */

    seq_t nn = (seq_t)n;        /* promote once for speed */

    /* k is guaranteed to exist within [2, n+2] */
    for (idx_t k = 2; k <= n + 2; ++k) {
        seq_t kk = (seq_t)k;
        seq_t denom = kk + nn;          /* k + n   */
        seq_t numer = kk * kk + nn;     /* k^2 + n */
        if ((numer % denom) == 0u)      /* divisibility test */
            return kk;                  /* first (smallest) k found */
    }

    /* Should never be reached; included for completeness */
    return 0;
}
