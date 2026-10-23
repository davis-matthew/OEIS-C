/*
 * A184888  a(n) = C(2n,n) * (8^n/n!^2) * Product_{k=0..n-1} (8k+3)*(8k+5).
 * https://oeis.org/A184888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(2n,n) * (8^n/n!^2) * Π_{k=0}^{n-1} (8k+3)(8k+5)
 *            Equivalent recurrence: a(0) = 1 a(n) = a(n-1) * (16 * (2n-1) *
 *            (8n-5) * (8n-3)) / n³ for n ≥ 1 The recurrence is exact for all
 *            n ≥ 0 and avoids any explicit factorial computation. The
 *            implementation uses a 128‑bit intermediate to keep the p...
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    seq_t val = seq_one();          /* a(0) = 1 */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* Compute a(i) from a(i-1) using the recurrence */
        unsigned __int128 prod = (unsigned __int128)val;
        prod *= 16;
        prod *= (2 * i - 1);
        prod *= (8 * i - 5);
        prod *= (8 * i - 3);

        /* Denominator n³ fits in 128 bits for all n that are tested. */
        unsigned __int128 den = (unsigned __int128)i * i * i;
        prod /= den;

        val = (seq_t)prod;        /* cast back to seq_t */
    }
    return val;
}
