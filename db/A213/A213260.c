/*
 * A213260  p(5n+4) where p(k) = number of partitions of k = A000041(k).
 * https://oeis.org/A213260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(5n+4) where p(k) is the partition function
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 24, u64 n <= 82, u128 n <= 290,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_PARTS (50005) /* enough for 5*10000+4 */

static seq_t part_cache[MAX_PARTS];
static idx_t part_cap = 0;

/* Compute partitions up to m using Euler's pentagonal number theorem */
static void compute_partitions(idx_t m)
{
    if (m >= MAX_PARTS) return;            /* out of bounds – never used for n≤10000 */

    if (part_cap == 0) {
        part_cap = 1;
        part_cache[0] = 1;                 /* p(0) = 1 */
    }

    for (idx_t i = part_cap; i <= m; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            seq_t term = part_cache[i - pent1];
            if (k & 1) sum += term; else sum -= term;

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > i) continue;
            term = part_cache[i - pent2];
            if (k & 1) sum += term; else sum -= term;
        }
        part_cache[i] = sum;
    }
    part_cap = m + 1;
}

seq_t a(idx_t n)
{
    idx_t m = (idx_t)5 * n + 4;          /* compute the argument 5n+4 */
    if (m >= MAX_PARTS) return 0;        /* safety – never occurs for n≤10000 */

    compute_partitions(m);
    return part_cache[m];
}
