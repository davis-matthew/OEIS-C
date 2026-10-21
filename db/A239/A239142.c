/*
 * A239142  Number of strict partitions of n having standard deviation sigma >
 *          1.
 * https://oeis.org/A239142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000009(n) - (1 + [n>=3] + [n%3==0 && n>=6]), where
 *            A000009(n) is the number of strict (distinct) partitions of n
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array for strict partition counts up to n. */
    size_t size = (size_t)n + 1;
    seq_t *part = (seq_t *)calloc(size, sizeof(seq_t));
    if (!part) return 0;          /* allocation failure yields 0 */

    part[0] = 1;                  /* empty partition */

    /* Dynamic programming: for each possible part, update sums decreasingly. */
    for (idx_t part_val = 1; part_val <= n; ++part_val) {
        for (idx_t sum = n; sum >= part_val; --sum) {
            part[sum] += part[sum - part_val];
        }
    }

    /* Count partitions with standard deviation <= 1. */
    seq_t bad = 1;                            /* the single‑part partition [n] */
    if (n >= 3) ++bad;                        /* the two‑part partition exists */
    if (n % 3 == 0 && n >= 6) ++bad;          /* the consecutive triple exists */

    seq_t result = part[n] - bad;             /* required value */

    free(part);
    return result;
}
