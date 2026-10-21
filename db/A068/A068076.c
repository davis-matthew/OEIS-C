/*
 * A068076  Number of positive integers < n with the same number of 1's in
 *          their binary expansions as n.
 * https://oeis.org/A068076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = #{ m < n | popcount(m) = popcount(n) }
 * Range:     u8 n <= 1052, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses binomial coefficients computed by binom(remaining, need). */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) 
        return 0;                 /* no positive integers < 0 */

    /* popcount of n */
    idx_t pop_n = __builtin_popcountll((unsigned long long)n);
    seq_t count = 0;
    idx_t ones_seen = 0;

    /* iterate over bit positions from most significant to least */
    for (int pos = 63; pos >= 0; --pos)
    {
        if (n & ((idx_t)1 << pos))
        {
            idx_t remaining = pos;          /* number of lower bits */

            if (pop_n > ones_seen)
            {
                idx_t need = pop_n - ones_seen;
                if (need <= remaining)
                    count += binom(remaining, need);
            }
            ones_seen++;
        }
    }

    return count;
}
