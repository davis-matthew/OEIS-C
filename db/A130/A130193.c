/*
 * A130193  a(0)=1. a(n+1) = a(ceiling(n/a(n))) + 1.
 * https://oeis.org/A130193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1; a(n+1)=a(ceil(n/a(n)))+1
 * Range:     u8 n <= 9755, u16 n <= 9755, u32 n <= 9755, u64 n <= 9755,
 *            u128 n <= 9755, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes A130193.  The function may be called in any order.
   It lazily computes values and keeps them in a static array. */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;     /* Array of already computed terms    */
    static idx_t last = 0;        /* Highest index that has been computed */

    /* Allocate and initialise the zeroth element on first call */
    if (!seq)
    {
        seq = (seq_t *)malloc(sizeof(seq_t));
        seq[0] = 1;          /* a(0) = 1 */
        last = 0;
    }

    /* If the requested index is beyond what we have, extend the array
       and compute the new terms. */
    if (n > last)
    {
        /* Allocate exactly enough space for indices 0..n */
        seq_t *tmp = (seq_t *)realloc(seq, (n + 1) * sizeof(seq_t));
        if (!tmp) { /* Allocation failed – abort: no meaningful return */ }
        seq = tmp;

        for (idx_t i = last + 1; i <= n; ++i)
        {
            idx_t prevIdx = i - 1;          /* index of a(i-1)          */
            seq_t prevValSeq = seq[prevIdx];/* a(i-1) in seq_t          */
            idx_t prevVal = (idx_t)prevValSeq;  /* convert to idx_t        */

            /* ceil(prevIdx / prevVal) using integer arithmetic */
            idx_t ceilDiv = (prevIdx + prevVal - 1) / prevVal;

            seq[i] = seq[ceilDiv] + 1;     /* recurrence definition   */
        }

        last = n;
    }

    return seq[n];
}
