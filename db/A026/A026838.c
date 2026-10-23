/*
 * A026838  Number of partitions of n into distinct parts, the greatest being
 *          even.
 * https://oeis.org/A026838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts whose
 *            greatest part is even
 * Range:     u8 n <= 33, u16 n <= 85, u32 n <= 250, u64 n <= 813,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>


seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1, safe default */

    /* Convert to size_t for array indexing */
    size_t N = (size_t)n;

    /* Allocate DP arrays: total = all partitions into distinct parts,
       even   = partitions whose largest part is even,
       odd    = partitions whose largest part is odd. */
    seq_t *total = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *even  = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *odd   = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!total || !even || !odd) {
        free(total); free(even); free(odd);
        return 0;
    }

    total[0] = 1;  /* empty partition */

    /* Enumerate parts from 1 to n. Each part can be used at most once. */
    for (idx_t part = 1; part <= n; ++part) {
        size_t part_sz = (size_t)part;

        /* Iterate i from N down to part_sz (descending).  Use size_t and a break
           to avoid unsigned underflow. */
        for (size_t i = N; ; --i) {
            size_t idx = i - part_sz;   /* index of remaining sum */
            seq_t add = total[idx];     /* partitions of i-part_sz using smaller parts */

            if ((part_sz & 1ULL) == 0ULL) {  /* part is even -> new largest part is even */
                even[i] += add;
            } else {                         /* part is odd -> new largest part is odd */
                odd[i] += add;
            }
            total[i] += add;   /* update total partitions */

            if (i == part_sz) break;  /* terminate inner loop */
        }
    }

    seq_t result = even[N];   /* partitions of n with largest part even */

    free(total); free(even); free(odd);
    return result;
}
