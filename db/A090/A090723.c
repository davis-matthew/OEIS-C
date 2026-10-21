/*
 * A090723  a(1) = 1; for n > 1, number of partitions of n into distinct odd
 *          parts such that the two largest parts differ by 2.
 * https://oeis.org/A090723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = B(n) - B(n-2), a(1) = 1, a(2) = 0 where B(n) = number of
 *            partitions of n into distinct odd parts. Compute B by dynamic
 *            programming over odd parts: B[0] = 1, B[s] = sum_{odd p <= s}
 *            B[s-p] (each odd part used at most once)
 * Range:     u8 n <= 98, u16 n <= 224, u32 n <= 603, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdlib.h>

/* Global cache for B(n) values */
static seq_t *B_cache = NULL;
static idx_t B_max = 0;   /* largest n for which B_cache is valid */

/* Compute B(n) for all n up to limit, rebuild cache from scratch */
static void compute_B_up_to(idx_t limit)
{
    if (limit <= B_max)
        return;

    /* Allocate new array and compute all values from scratch */
    seq_t *newB = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    if (!newB)
        abort();
    newB[0] = (seq_t)1;               /* empty partition */
    for (idx_t i = 1; i <= limit; ++i)
        newB[i] = (seq_t)0;

    for (idx_t p = 1; p <= limit; p += 2)          /* iterate odd parts */
    {
        for (idx_t s = limit; s >= p; --s)         /* descending to keep parts distinct */
            newB[s] += newB[s - p];
    }

    /* Replace old cache */
    free(B_cache);
    B_cache = newB;
    B_max    = limit;
}

/* OEIS sequence A090723 */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)0;

    /* Ensure B(n) and B(n-2) are computed up to n */
    compute_B_up_to(n);

    return B_cache[n] - B_cache[n - 2];
}
