/*
 * A131553  a(n) = Product_{k=1..n, gcd(k,n)=1} (1+k).
 * https://oeis.org/A131553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n, gcd(k,n)=1} (1 + k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 12, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 */

#include "../utils/gcd.h"

/* Compute the sequence element a(n) for a(0)=1 */
seq_t a(idx_t n)
{
    if (n == 0)                       /* by definition a(0)=1 */
        return 1;

    seq_t prod = 1;

    /* iterate over 1..n, multiply (k+1) when k is coprime to n */
    for (idx_t k = 1; k <= n; ++k)
    {
        if (gcd((seq_t)k, (seq_t)n) == 1)
            prod *= ((seq_t)k + 1);
    }
    return prod;
}
