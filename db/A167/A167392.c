/*
 * A167392  Characteristic function of partition numbers.
 * https://oeis.org/A167392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167392
   a(n) = 1  if n occurs in the partition numbers A000041,
          0  otherwise.  */
#include <stdlib.h>

/* static storage for partition numbers p(k) (k>=0) */
static idx_t *part = NULL;   /* array of partition values */
static size_t part_len = 0;  /* number of elements stored */
static size_t part_cap = 0;  /* allocated capacity */

/* initialise the array with p(0)=1 and grow it when necessary */
static void init_partitions(void)
{
    if (part == NULL) {
        part_cap = 16;
        part = (idx_t *)malloc(part_cap * sizeof(idx_t));
        part[0] = (idx_t)1;          /* p(0) = 1 */
        part_len = 1;
    }
}

/* compute partition numbers until the last one exceeds n */
static void ensure_partitions(idx_t n)
{
    init_partitions();
    /* grow until the last computed partition is > n */
    while (part[part_len-1] <= n) {
        idx_t k = part_len;          /* next index to compute */
        idx_t sum = 0;

        /* generalized pentagonal number theorem */
        for (int j = 1; ; ++j) {
            idx_t pent1 = (idx_t)j * (3*j - 1) / 2;
            if (pent1 > k) break;    /* stop when out of range */
            if (j & 1) sum += part[k - pent1];
            else       sum -= part[k - pent1];

            idx_t pent2 = (idx_t)j * (3*j + 1) / 2;
            if (pent2 > k) continue;
            if (j & 1) sum += part[k - pent2];
            else       sum -= part[k - pent2];
        }

        /* grow array if needed */
        if (part_len == part_cap) {
            part_cap <<= 1;
            part = (idx_t *)realloc(part, part_cap * sizeof(idx_t));
        }
        part[part_len++] = sum;
    }
}

/* binary search to test whether n is a partition number */
static int is_partition(idx_t n)
{
    size_t lo = 0, hi = part_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (part[mid] < n) lo = mid + 1;
        else               hi = mid;
    }
    return (lo < part_len && part[lo] == n) ? 1 : 0;
}

/* required public function */
seq_t a(idx_t n)
{
    /* 0 is never a partition number (p(0)=1) */
    if (n == 0) return 0;

    ensure_partitions(n);
    return is_partition(n);
}
