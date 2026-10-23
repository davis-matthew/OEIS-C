/*
 * A026086  Number of (s(0), s(1), ..., s(n)) such that s(0) = 0, |s(i) -
 *          s(i-1)| = 1 for i = 1,2,3; |s(i) - s(i-1)| <= 1 for i >= 4, s(n) =
 *          3; also a(n) = T(n,n-3), where T is the array defined in A026082.
 * https://oeis.org/A026086
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of sequences (s(0)…s(n)) with s(0)=0,
 *            |s(i)-s(i-1)|=1 for i=1,2,3 and |s(i)-s(i-1)|≤1 for i≥4, ending
 *            with s(n)=3. Implemented via dynamic programming
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 4)                     /* offset is 4, values below are undefined */
        return 0;                  /* return 0 (any seq_t zero is fine) */

    const size_t len   = (size_t)(2 * n + 1);   /* positions from -n to n */
    const size_t offset = (size_t)n;            /* index of value 0 */

    seq_t *cur  = (seq_t*)malloc(len * sizeof(seq_t));
    seq_t *next = (seq_t*)malloc(len * sizeof(seq_t));
    if (!cur || !next)
        return 0; /* memory failure – treat as zero (unlikely in tests) */

    /* initialise */
    for (size_t i = 0; i < len; ++i) cur[i] = 0;
    cur[offset] = 1;                         /* s(0)=0 */

    for (idx_t step = 1; step <= n; ++step)
    {
        /* zero next array */
        for (size_t i = 0; i < len; ++i) next[i] = 0;

        /* allowed step sizes for this step */
        int step_size[3];
        int k;
        if (step <= 3) {
            step_size[0] = -1;
            step_size[1] =  1;
            k = 2;
        } else {
            step_size[0] = -1;
            step_size[1] =  0;
            step_size[2] =  1;
            k = 3;
        }

        for (size_t idx = 0; idx < len; ++idx)
        {
            seq_t ways = cur[idx];
            if (ways == 0) continue;       /* skip zero counts */

            for (int t = 0; t < k; ++t)
            {
                const int target = (int)idx + step_size[t];
                /* target is guaranteed to lie in [0, 2n] */
                next[target] = next[target] + ways;
            }
        }

        /* swap cur and next buffers for next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    seq_t result = cur[offset + 3];          /* s(n) must be 3 */

    free(cur);
    free(next);
    return result;
}
