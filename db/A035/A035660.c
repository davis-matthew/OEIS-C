/*
 * A035660  Number of partitions of n into parts 7k+1 and 7k+5 with at least
 *          one part of each type.
 * https://oeis.org/A035660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into parts 7k+1 and 7k+5 with * at
 *            least one part of each type. The algorithm builds three *
 *            dynamic‑programming tables: * dp1 – partitions using only parts
 *            ≡ 1 (mod 7) * dp5 – partitions using only parts ≡ 5 (mod 7) *
 *            dp12 – partitions using any allowed part * The desir...
 * Range:     u8 n <= 50, u16 n <= 133, u32 n <= 407, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                     /* size_t */
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array(size_t n) -> seq_t* */
#include "../utils/free_seq.h"          /* free_seq(seq_t *p) */

seq_t a(idx_t n)
{
    if (n == 0)          /* the sequence is 1‑based; a(0) is unused */
        return 0;

    const size_t N = ((size_t)n) + 1;

    seq_t *dp1  = alloc_seq_array(N);
    seq_t *dp5  = alloc_seq_array(N);
    seq_t *dp12 = alloc_seq_array(N);

    /* Base case: one way to partition 0 (empty partition). */
    dp1[0]  = 1;
    dp5[0]  = 1;
    dp12[0] = 1;

    /* Fill tables, iterating over all admissible part sizes. */
    for (idx_t part = 1; part <= n; ++part)
    {
        if ((part % 7 == 1) || (part % 7 == 5))
        {
            /* Add this part to the generic table. */
            for (idx_t i = part; i <= n; ++i)
                dp12[i] += dp12[i - part];

            if (part % 7 == 1)
            {
                /* Update the table that tracks only 1‑mod‑7 parts. */
                for (idx_t i = part; i <= n; ++i)
                    dp1[i] += dp1[i - part];
            }
            else   /* part % 7 == 5 */
            {
                /* Update the table that tracks only 5‑mod‑7 parts. */
                for (idx_t i = part; i <= n; ++i)
                    dp5[i] += dp5[i - part];
            }
        }
    }

    seq_t result = dp12[n] - dp1[n] - dp5[n];

    free_seq(dp1);
    free_seq(dp5);
    free_seq(dp12);

    return result;
}
