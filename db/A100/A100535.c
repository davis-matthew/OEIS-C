/*
 * A100535  Number of partitions of 2*n + 1 into parts of two kinds.
 * https://oeis.org/A100535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000712(2*n+1) = sum_{k=0}^{2*n+1} p(k) * p(2*n+1-k),
 *            where p(k) is the partition number of k
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 33, u64 n <= 107, u128 n <= 373,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *p = NULL;          /* partition numbers up to current limit */
    static idx_t p_size = 0;         /* allocated size of p (index + 1) */

    idx_t max_needed = 2 * n + 1;    /* largest k needed for a(n) */

    if (max_needed + 1 > p_size) {  /* need to grow the array */
        idx_t new_size = max_needed + 1;
        seq_t *tmp = (seq_t *)realloc(p, new_size * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed; return 0 (undefined) */
        p = tmp;
        p_size = new_size;
    }

    /* Compute partition numbers p[0 .. max_needed] if not already done.
       We recompute all entries for simplicity; the cost is negligible for
       max_needed <= 20001. */
    p[0] = 1;
    for (idx_t i = 1; i <= max_needed; ++i)
    {
        seq_t pos = 0;   /* sum of positive terms */
        seq_t neg = 0;   /* sum of negative terms */

        for (idx_t k = 1; ; ++k)
        {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;

            if (k & 1)       /* (+) for odd k */
                pos += p[i - pent1];
            else             /* (-) for even k */
                neg += p[i - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > i) continue;

            if (k & 1)       /* same sign as pent1 */
                pos += p[i - pent2];
            else
                neg += p[i - pent2];
        }
        p[i] = pos - neg;  /* guaranteed non‑negative for partition numbers */
    }

    /* Compute a(n) = sum_{k} p(k) * p(max_needed - k) */
    seq_t result = 0;
    for (idx_t k = 0; k <= max_needed; ++k)
        result += p[k] * p[max_needed - k];

    return result;
}
