/*
 * A049610  a(n) = Sum_{k=0..floor(n/2)} k*binomial(n,2*k) = floor(n*2^(n-3)).
 * https://oeis.org/A049610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*2^(n-3)) for n >= 0
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 61, u128 n <= 124,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 3)
    {
        switch (n)
        {
            case 0:
                return (seq_t)0;
            case 1:
                return (seq_t)0;
            case 2:
                return (seq_t)1;
        }
    }
    return (seq_t)n * pow2(n - 3);
}
