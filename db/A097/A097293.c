/*
 * A097293  Contains exactly once every triple i,j,k such that 0<i<j<k.
 * https://oeis.org/A097293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = the n‑th element of the list of all triples (i, j, k)
 *            with 0 < i < j < k, ordered first by the sum i + j + k
 *            (ascending) and, within equal sums, by lexicographic order of
 *            (i, j, k). The sequence consists of the individual numbers i, j
 *            and k from each triple (too see a(1)=1, a(2)=2, a(3)=3,...
 * Range:     u8 n <= 9439, u16 n <= 9439, u32 n <= 9439, u64 n <= 9439,
 *            u128 n <= 9439, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n < 1, return 0 */
        return 0;

    idx_t count = 0;            /* how many terms seen so far */

    for (idx_t sum = 6; ; ++sum)          /* minimal admissible sum */
    {
        for (idx_t i = 1; i <= sum - 3; ++i)
        {
            for (idx_t j = i + 1; j <= sum - i - 1; ++j)
            {
                idx_t k = sum - i - j;
                if (k <= j)          /* enforce j < k */
                    continue;

                /* i */
                ++count;
                if (count == n)
                    return (seq_t)i;

                /* j */
                ++count;
                if (count == n)
                    return (seq_t)j;

                /* k */
                ++count;
                if (count == n)
                    return (seq_t)k;
            }
        }
    }
}
