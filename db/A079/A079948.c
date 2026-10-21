/*
 * A079948  First differences of A079000.
 * https://oeis.org/A079948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(1)=3, a(2)=2; for n>2: the values are 1 or 2.
 * The sequence consists of runs of length 3·2^k of 1's followed by
 * runs of the same length of 2's for k = 0,1,2,...
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)3;
    if (n == (idx_t)2)
        return (seq_t)2;

    idx_t j = n - (idx_t)2;          /* position within the repeat pattern */
    unsigned k = 0;

    for (;; ++k)
    {
        /* blockSize = 6 * 2^k, halfSize = 3 * 2^k; use 128‑bit to avoid overflow */
        unsigned __int128 blockSize = ((unsigned __int128)6ULL) << k;
        unsigned __int128 halfSize  = ((unsigned __int128)3ULL) << k;
        unsigned __int128 uj = (unsigned __int128)j;

        if (uj <= blockSize)      /* the term lies in this cycle */
        {
            if (uj <= halfSize)   /* the first half of the cycle */
                return (seq_t)1;
            else                  /* the second half of the cycle */
                return (seq_t)2;
        }

        /* otherwise skip this full cycle (blockSize <= j) */
        j -= (idx_t)blockSize;     /* blockSize fits in idx_t because it is <= j here */
    }
}
