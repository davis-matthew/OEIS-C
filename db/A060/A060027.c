/*
 * A060027  Expansion of (1-x-x^N)/((1-x)(1-x^2)(1-x^3)...(1-x^N)) for N = 8.
 * https://oeis.org/A060027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = p(n) - p(n-1) - p(n-8) where p(k) = number of partitions
 *            of k into parts 1..8
 * Range:     i8 n <= 27, i16 n <= 60, i32 n <= 322, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *partitions = NULL;   /* p(k) for k = 0..max_n */
static idx_t   part_size  = 0;     /* current allocated size */

/* Recompute the partition table up to max_n (inclusive).  */
static void compute_partitions(idx_t max_n)
{
    /* allocate new array */
    seq_t *newp = (seq_t *)malloc((max_n + 1) * sizeof(seq_t));
    if (!newp) abort();                 /* out of memory */

    /* initialise */
    for (idx_t i = 0; i <= max_n; ++i) newp[i] = 0;
    newp[0] = 1;                         /* one partition of 0 */

    /* unlimited parts 1..8 */
    for (idx_t part = 1; part <= 8; ++part) {
        for (idx_t sum = part; sum <= max_n; ++sum) {
            newp[sum] += newp[sum - part];
        }
    }

    /* replace old table */
    free(partitions);
    partitions = newp;
    part_size  = max_n + 1;
}

seq_t a(idx_t n)
{
    /* Ensure we have p(k) up to n (and n-1, n-8). */
    if (n >= part_size) {
        compute_partitions(n);
    }

    seq_t res = partitions[n];
    if (n >= 1)               res -= partitions[n - 1];
    if (n >= 8)               res -= partitions[n - 8];
    return res;
}
