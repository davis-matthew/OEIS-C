/*
 * A182724  Sum of all parts of all partitions of n minus the number of
 *          partitions of n.
 * https://oeis.org/A182724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1) * p(n) (p(n) = number of partitions of n)
 * Range:     u8 n <= 9, u16 n <= 26, u32 n <= 88, u64 n <= 326,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *part_cache = NULL;
static idx_t   part_cap   = 0;
static idx_t   part_last  = 0;

/* Ensure partition numbers are computed up to n (inclusive) */
static void ensure_partitions(idx_t n)
{
    if (n <= part_last) return;

    /* Allocate or expand cache */
    if (!part_cache) {
        part_cap = (n >= 16) ? n + 1 : 16;
        part_cache = (seq_t *)malloc(part_cap * sizeof(seq_t));
        if (!part_cache) exit(1);
        for (idx_t i = 0; i < part_cap; ++i) part_cache[i] = 0;
        part_cache[0] = 1;          /* p(0) = 1 */
        part_last = 0;
    } else if (n + 1 > part_cap) {
        size_t new_cap = part_cap;
        while (new_cap <= n) new_cap <<= 1;
        part_cache = (seq_t *)realloc(part_cache, new_cap * sizeof(seq_t));
        if (!part_cache) exit(1);
        for (idx_t i = part_cap; i < new_cap; ++i) part_cache[i] = 0;
        part_cap = new_cap;
    }

    /* Compute missing values */
    for (idx_t i = part_last + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = (k * (3 * k - 1)) >> 1;   /* k(3k-1)/2 */
            if (pent1 > i) break;
            seq_t val = part_cache[i - pent1];
            if (k & 1) sum += val;
            else      sum -= val;

            idx_t pent2 = (k * (3 * k + 1)) >> 1;   /* k(3k+1)/2 */
            if (pent2 > i) continue;
            val = part_cache[i - pent2];
            if (k & 1) sum += val;
            else      sum -= val;
        }
        part_cache[i] = sum;
    }
    part_last = n;
}

/* Partition number p(n) */
static seq_t partition(idx_t n)
{
    ensure_partitions(n);
    return part_cache[n];
}

/* OEIS sequence A182724 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n=0, safety only */
    seq_t pn = partition(n);
    seq_t nn = (seq_t)(n - 1);
    return nn * pn;
}
