/*
 * A066382  a(n) = Sum_{k=0..n} binomial(n^2,k).
 * https://oeis.org/A066382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n^2,k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* N = n^2, fits in idx_t for all expected inputs */
    idx_t N = n * n;

    seq_t sum = 1;   /* binomial(N,0) */
    seq_t c   = 1;   /* current binomial value */

    for (idx_t k = 1; k <= n; ++k)
    {
#if defined(__SIZEOF_INT128__)
        /* When seq_t is a builtin integer (≤ 64‑bit on most builds),
           use a 128‑bit temporary to avoid overflow in the intermediate
           multiplication.  The division is exact. */
        if (sizeof(seq_t) <= 8)
        {
            __int128 num = (__int128)c * (N - k + 1);
            c = (seq_t)(num / k);
        }
        else
#endif
        {
            /* For arbitrary‑precision seq_t, the built‑in operators are
               sufficient and exact. */
            c = (c * (seq_t)(N - k + 1)) / (seq_t)k;
        }
        sum += c;
    }
    return sum;
}
