/*
 * A121641  a(0) = 1. a(n) = a(n-1) + (n-th integer from among those positive
 *          integers which are coprime to a(n-1)).
 * https://oeis.org/A121641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 279, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n>0: a(n)=a(n-1)+c_n, where c_n is the n‑th positive integer coprime to a(n-1). */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t val = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t candidate = (seq_t)1;
        idx_t count = 0;
        while (1)
        {
            if (gcd(val, candidate) == (seq_t)1)
            {
                ++count;
                if (count == k) break;
            }
            ++candidate;
        }
        val = val + candidate;
    }
    return val;
}
