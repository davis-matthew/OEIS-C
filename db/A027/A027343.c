/*
 * A027343  Number of partitions of n that do not contain 9 as a part.
 * https://oeis.org/A027343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A027343: Number of partitions of n that do not contain 9 as a part.
    Formula: a(n) = A000041(n) - A000041(n-9)
    where A000041 is the partition function with p(0)=1, p(m)=0 for m<0.
    The sequence is computed by dynamic programming using only additions.  */

#include <stdlib.h>

/* Cache for ordinary partition numbers p(0..N).  */
static seq_t *part_cache = NULL;
static size_t cache_cap   = 0;

/* Recompute the partition numbers up to n (inclusive).  */
static void ensure_cache(idx_t n)
{
    size_t new_cap = (size_t)n + 1;

    /* Grow the array if necessary.  */
    if (new_cap > cache_cap)
    {
        seq_t *tmp = (seq_t *)realloc(part_cache, new_cap * sizeof(seq_t));
        part_cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise to zero.  */
    for (idx_t i = (idx_t)0; i <= n; ++i)
        part_cache[i] = (seq_t)0;

    /* p(0) = 1.  */
    part_cache[0] = (seq_t)1;

    /* Classic DP: for each part size k add its contribution to all sums j≥k.   */
    for (idx_t k = (idx_t)1; k <= n; ++k)
        for (idx_t j = k; j <= n; ++j)
            part_cache[j] += part_cache[j - k];
}

/* Public entry point for the sequence.  */
seq_t a(idx_t n)
{
    ensure_cache(n);

    if (n < (idx_t)9)
        return part_cache[n];

    return part_cache[n] - part_cache[n - (idx_t)9];
}
