/*
 * A078732  Numbers k such that the b1(k)^2=b0(k) where b1(k) denotes the
 *          number of 1's in binary expansion of k, b0(k) the number of 0's.
 * https://oeis.org/A078732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 61, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom_uint64, next_combination
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A078732: integers k for which popcount(k)^2 = number of zero bits in the binary
   representation of k.  For a given popcount b1 the binary length is
   L = b1*(b1+1).  All numbers with leading bit 1 and exactly b1 ones of
   length L satisfy the condition.  The function finds the n‑th such number
   by combinatorial counting and binary combination generation. */

#include <stdint.h>
#include "../utils/binom_uint64.h"
#include "../utils/next_combination.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe */

    idx_t remaining = n;           /* terms still needed */
    for (uint32_t b1 = 1; ; ++b1)
    {
        uint32_t L          = b1 * (b1 + 1);   /* bit length of numbers with popcount b1 */
        uint32_t lower_bits = L - 1;          /* bits below the leading 1        */
        uint32_t need       = b1 - 1;         /* additional ones to place         */

        uint64_t term_count = binom_uint64((uint64_t)lower_bits, need);

        if (remaining > term_count)
        {
            remaining -= term_count;   /* skip this b1 block */
            continue;
        }

        /* Rank (0‑based) of desired combination among those with this b1 */
        uint64_t rank = remaining - 1;
        uint64_t mask = 0;

        if (need > 0)
        {
            mask = (1ULL << need) - 1;       /* first combination */
            for (uint64_t i = 0; i < rank; ++i)
                next_combination(&mask, need);
        }

        /* Build the integer: leading 1 at position L-1 plus the chosen mask. */
        seq_t val = ((seq_t)1) << (L - 1);
        val += (seq_t)mask;
        return val;
    }
}
