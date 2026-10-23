/*
 * A161044  Number of partitions of n into odd numbers where every part
 *          appears at least 8 times.
 * https://oeis.org/A161044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 124, u16 n <= 301, u32 n <= 853, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of partitions of n into odd parts where each part
   appears at least 8 times.  The routine uses dynamic programming over
   odd part sizes.  For each odd j the update is
     new[s] = old[s] + ∑_{k≥8} old[s − k·j] .
   This is implemented by building the inner sum incrementally in
   'temp', achieving O(n²/2) time and O(n) space. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    size_t N = (size_t)n + 1;

    /* Allocate three temporary arrays of seq_t.  Cast the void* from
       calloc/malloc because seq_t may be a user‑defined type in C++. */
    seq_t *old    = (seq_t*)calloc(N, sizeof(seq_t));
    seq_t *newdp  = (seq_t*)calloc(N, sizeof(seq_t));
    seq_t *temp   = (seq_t*)calloc(N, sizeof(seq_t));

    old[0] = (seq_t)1;          /* empty partition */

    for (idx_t j = 1; j <= n; j += 2)          /* odd part sizes only */
    {
        /* Build contributions that use part j at least 8 times. */
        for (idx_t s = 8 * j; s <= n; ++s)
        {
            seq_t val = old[s - 8 * j];
            if (s >= j)
                val += temp[s - j];
            temp[s] = val;
        }

        /* Combine with partitions that do not use part j. */
        for (idx_t s = 0; s <= n; ++s)
        {
            newdp[s] = old[s] + temp[s];
            temp[s]  = (seq_t)0;   /* reset for next iteration */
        }

        /* Swap pointers for next round. */
        seq_t *p = old;
        old     = newdp;
        newdp   = p;
    }

    seq_t result = old[n];

    free(old);
    free(newdp);
    free(temp);

    return result;
}
