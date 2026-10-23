/*
 * A168160  Number of 0's in the matrix whose lines are the binary expansion
 *          of the numbers 1,...,n.
 * https://oeis.org/A168160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n*A070939(n) - A000788(n) where A070939(n) =
 *            floor_log2(n)+1 and A000788(n) = sum_{k=1}^n popcount(k)
 * Range:     u8 n <= 64, u16 n <= 8665, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute the number of 0's in the binary matrix for 1..n */
#include <stdint.h>   /* for UINT64_MAX */

seq_t a(idx_t n)
{
    /* number of bit positions needed for the matrix */
    idx_t bits = 0;
    for (idx_t x = n; x; x >>= 1)
        ++bits;             /* bits >= 1 for n >= 1 */

    /* sum of popcounts of all numbers 1..n */
    seq_t sumOnes = 0;
    for (idx_t i = 0; i < bits; ++i)
    {
        if (i == 63)          /* special handling for the most significant bit */
        {
            seq_t ones_i = 0;
            if (n >= ((idx_t)1 << 63))
                /* numbers from 2^63 to n inclusive */
                ones_i = ((seq_t)n - ((seq_t)1 << 63) + 1);
            sumOnes += ones_i;
            continue;
        }

        /* cycle length for bit i: 2^(i+1) */
        idx_t cycle = ((idx_t)1 << (i + 1));
        idx_t half  = ((idx_t)1 << i);

        /* compute (n+1)/cycle and (n+1)%cycle without overflow */
        idx_t full  = n / cycle;
        idx_t rem   = n % cycle;
        if (rem + 1 >= cycle) {         /* carry over when (n+1) exceeds cycle */
            ++full;
            rem = rem + 1 - cycle;
        } else {
            rem = rem + 1;
        }

        /* ones contributed by full cycles and remainder */
        idx_t ones_in_full = full * half;
        idx_t extra = (rem > half) ? (rem - half) : 0;
        idx_t ones_i = ones_in_full + extra;

        sumOnes += (seq_t)ones_i;
    }

    /* total bits written in the matrix minus ones = zeros */
    seq_t totalBits = (seq_t)n * (seq_t)bits;
    return totalBits - sumOnes;
}
