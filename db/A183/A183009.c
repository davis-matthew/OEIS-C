/*
 * A183009  a(n) = 24*n*p(n) = 24*n*A000041(n).
 * https://oeis.org/A183009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*n*A000041(n)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"   /* seq_t* alloc_seq_array(size_t n); */

static seq_t compute_partition(idx_t n)
{
    /* Allocate array part[0..n] */
    seq_t *part = alloc_seq_array((size_t)n + 1);
    if (!part) exit(1);                 /* allocation failure */

    part[0] = 1;                         /* p(0) = 1 */

    for (size_t i = 1; i <= (size_t)n; ++i)
    {
        seq_t pos = 0, neg = 0;          /* running sums for positive and negative terms */

        for (int k = 1; ; ++k)
        {
            int pent1 = k * (3 * k - 1) / 2;
            if (pent1 > (int)i) break;   /* no more contributions for this i */

            int sign = (k & 1) ? 1 : -1; /* + for odd k, - for even k */

            if (sign > 0)
                pos += part[i - pent1];
            else
                neg += part[i - pent1];

            int pent2 = k * (3 * k + 1) / 2;
            if (pent2 > (int)i) continue;/* pent2 might be beyond i */

            if (sign > 0)
                pos += part[i - pent2];
            else
                neg += part[i - pent2];
        }

        /* p(i) = pos - neg; pos >= neg for all i */
        part[i] = pos - neg;
    }

    seq_t result = part[(size_t)n];
    free(part);   /* clean up allocated array */
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* sequence starts at offset 1 */

    seq_t p_n = compute_partition(n);   /* p(n) */

    seq_t res = (seq_t)n;               /* promote n to seq_t */
    res *= 24;                          /* multiply by 24 */
    res *= p_n;                         /* multiply by p(n) */

    return res;
}
