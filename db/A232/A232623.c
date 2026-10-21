/*
 * A232623  Number of partitions of 2*n into parts with multiplicity <= n.
 * https://oeis.org/A232623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 21, u32 n <= 63, u64 n <= 208, u128 n <= 729,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = p(2*n) - p(n-1)
 * where p(k) is the partition number of k
 * (number of partitions of 2*n with parts <= n).
 * This implements the recurrence via the pentagonal number theorem.
 */

#include <stddef.h>
#include <stdlib.h>

/* static storage for partition numbers p[0] .. */
static seq_t *part = NULL;
static size_t part_cap = 0;

/* Ensure that partition numbers p[0 .. N] have been computed. */
static void
ensure_partitions(idx_t N)
{
    /* Number of entries needed is N+1 (including 0). */
    size_t need = (size_t)N + 1;
    if (need <= part_cap)            /* already available */
        return;

    /* Reallocate storage to the needed size. */
    seq_t *tmp = (seq_t *)realloc(part, need * sizeof(seq_t));
    part = tmp;

    /* Initialise newly allocated entries to zero. */
    for (size_t i = part_cap; i < need; ++i)
        part[i] = 0;

    /* Base case p[0] = 1. */
    if (part_cap == 0) {
        part[0] = 1;
        part_cap = 1;
    }

    /* Compute partition numbers from the next unused index. */
    size_t start = part_cap;
    if (start < 1)
        start = 1;

    for (size_t i = start; i < need; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > (idx_t)i)
                break;
            if (k & 1)
                pos += part[i - (size_t)g1];
            else
                neg += part[i - (size_t)g1];

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > (idx_t)i)
                continue;
            if (k & 1)
                pos += part[i - (size_t)g2];
            else
                neg += part[i - (size_t)g2];
        }
        part[i] = pos - neg;   /* pos >= neg for all i */
    }

    part_cap = need;
}

/* Public sequence function */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 1;  /* a(0) = 1 */

    /* Need partitions up to 2*n (and n-1, which is <= 2*n). */
    ensure_partitions(2 * n);

    return part[2 * n] - part[n - 1];
}
