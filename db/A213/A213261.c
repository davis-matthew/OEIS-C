/*
 * A213261  a(n) = p(7*n + 5), where p(k) = number of partitions of k =
 *          A000041(k).
 * https://oeis.org/A213261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(7*n + 5) where p(k) is the partition number of k
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 17, u64 n <= 58, u128 n <= 207,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static state for memoisation of partition numbers */
static seq_t *part = NULL;          /* array of partitions, indexed by non‑negative integer */
static idx_t  part_capacity = 0;    /* allocated capacity (in elements) */
static idx_t  part_computed  = 0;   /* largest k for which part[k] is known */

/* enlarge the partition table to accommodate at least new_size items */
static void grow_part(idx_t new_size)
{
    if (new_size <= part_capacity)
        return;

    seq_t *new_part = (seq_t *)realloc(part, new_size * sizeof(seq_t));
    /* initialise the new slots to zero (leaving the existing values untouched) */
    for (idx_t i = part_capacity; i < new_size; ++i)
        new_part[i] = 0;
    part = new_part;
    part_capacity = new_size;

    /* initialise p(0) */
    if (part_computed == 0) {
        part[0] = 1;
        part_computed = 0;
    }
}

seq_t a(idx_t n)
{
    /* we need the partition number of k = 7*n + 5 */
    idx_t k = n * 7 + 5;

    /* make sure the partition table can hold p(k) */
    if (part == NULL || part_capacity <= k)
        grow_part(k + 1);

    /* compute any missing partition numbers up to k */
    for (idx_t i = part_computed + 1; i <= k; ++i) {
        seq_t sum_pos = 0;   /* sum of terms with + sign   */
        seq_t sum_neg = 0;   /* sum of terms with - sign   */

        for (idx_t m = 1; ; ++m) {
            /* generalized pentagonal numbers g1 = m*(3m-1)/2 and g2 = m*(3m+1)/2 */
            idx_t g1 = m * (3 * m - 1) / 2;
            if (g1 > i)
                break;
            if (m & 1)
                sum_pos += part[i - g1];
            else
                sum_neg += part[i - g1];

            idx_t g2 = m * (3 * m + 1) / 2;
            if (g2 <= i) {
                if (m & 1)
                    sum_pos += part[i - g2];
                else
                    sum_neg += part[i - g2];
            }
        }
        /* p(i) = sum_pos - sum_neg  (sum_pos >= sum_neg for all i >= 0) */
        part[i] = sum_pos - sum_neg;
    }

    part_computed = k;
    return part[k];
}
