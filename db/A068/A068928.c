/*
 * A068928  Number of incongruent ways to tile a 3 X 2n room with 1x2 Tatami
 *          mats. At most 3 Tatami mats may meet at a point.
 * https://oeis.org/A068928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - a(n-3) - a(n-5) - a(n-6) with a(1)=2,
 *            a(2)=2, a(3)=2, a(4)=4, a(5)=5, a(6)=9, a(7)=12
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 48, u64 n <= 94, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: a[0] unused, a[1..] store sequence values. */
    static seq_t *seq = NULL;
    static idx_t sz = 0;        /* highest index currently stored */
    static const seq_t init[8] = {0,2,2,2,4,5,9,12};

    if (n < 1)                 /* behaviour for n<1 is irrelevant */
        return 0;

    /* Allocate or grow cache if necessary.  Allocate at least up to 7
       first so that recurrence indices exist. */
    if (sz < 7)
    {
        size_t new_sz = 8; /* indices 0..7 */
        seq = (seq_t *)realloc(seq, new_sz * sizeof(seq_t));
        for (idx_t i = 1; i <= 7; ++i)
            seq[i] = init[i];
        sz = 7;
    }

    /* Grow cache to reach requested n. */
    if (n > sz)
    {
        size_t new_sz = n + 1;               /* indices 0..n */
        seq_t *tmp = (seq_t *)realloc(seq, new_sz * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        seq = tmp;
        /* Compute new terms using recurrence.  Start from current sz+1. */
        for (idx_t i = sz + 1; i <= n; ++i)
        {
            if (i <= 7)
            {
                seq[i] = init[i];
            }
            else
            {
                /* All indices required are already computed. */
                seq[i] = seq[i-1]
                        + 2 * seq[i-2]
                        - seq[i-3]
                        - seq[i-5]
                        - seq[i-6];
            }
        }
        sz = n;
    }

    return seq[n];
}
