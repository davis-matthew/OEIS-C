/*
 * A087070  Sum{Floor(n*r^k): r=3/4, k>=0}.
 * https://oeis.org/A087070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥0} floor( n * (3/4)^k ) where the sum stops when the
 *            term becomes zero
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>        /* for size_t */

seq_t a(idx_t n)
{
    seq_t sum  = 0;
    seq_t pow3 = 1;               /* 3^k  (k starts at 0)                */
    const size_t bits = sizeof(seq_t) * 8;   /* number of addressable bits */

    for (idx_t k = 0; ; ++k)
    {
        size_t shift = 2 * (size_t)k;       /* denominator = 4^k = 2^(2k)        */
        if (shift >= bits)                  /* shift would be undefined             */
            break;

        seq_t term;
        /* use 128‑bit arithmetic when seq_t is 64 bits or less
           to avoid overflow of the product n * pow3    */
        if (sizeof(seq_t) <= sizeof(unsigned long long))
        {
            unsigned __int128 prod =
                (unsigned __int128)(unsigned long long)n *
                (unsigned __int128)(unsigned long long)pow3;
            term = (seq_t)(prod >> shift);
        }
        else
        {
            term = ((seq_t)n * pow3) >> (unsigned)shift;
        }

        if (!term)
            break;

        sum += term;
        pow3 *= 3;
    }

    return sum;
}
