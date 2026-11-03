/*
 * A168020  Square array read by antidiagonals in which row n lists the number
 *          of partitions of n into parts divisible by k.
 * https://oeis.org/A168020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 136, u16 n <= 946, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Validated: standard, pass, 107 terms through n = 1230 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS implementation of A168020.
 *
 *  The sequence is read by antidiagonals from the square array
 *  A(n,k) where
 *      A(n,k) = p(n/k) if k | n   (p = partition number)
 *      A(n,k) = 0 otherwise
 *
 *  This implementation computes the partition numbers by dynamic
 *  programming (no subtractions), thus works correctly for both
 *  unsigned 64‑bit and arbitrary‑precision seq_t values.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Compute the partition number p(m) for a non‑negative integer m.
 * Uses the recurrence
 *      p(0,k) = 1      for all k
 *      p(n,0) = 0      for n > 0
 *      p(n,k) = p(n,k-1)          if k > n
 *               p(n,k-1) + p(n-k,k) if k <= n
 *  The partition number p(m) is then p(m,m).  All arithmetic is done
 *  with seq_t additions only. */
static seq_t get_partition(idx_t m)
{
    if (m == 0)
        return (seq_t)1;

    size_t dim = (size_t)(m + 1);          /* dimension of the table  */
    seq_t *s = mem_new(seq_t, dim * dim); /* zero‑init */
    if (!s)
        exit(1);

    /* Base cases: p(0,k) = 1 for all k */
    for (size_t k = 0; k < dim; ++k)
        s[0 * dim + k] = (seq_t)1;

    /* Dynamic programming */
    for (size_t n = 1; n < dim; ++n) {
        for (size_t k = 1; k < dim; ++k) {
            seq_t val = s[n * dim + (k - 1)];      /* p(n,k-1)      */
            if (k <= n)
                val = val + s[(n - k) * dim + k]; /* + p(n-k,k)   */
            s[n * dim + k] = val;
        }
    }

    seq_t result = s[m * dim + m];
    mem_free(s);
    return result;
}

/*  Return a(n) for the OEIS sequence A168020, where n >= 1.
 *
 *  The mapping from the index n to the array position (row, col) is:
 *      d  = smallest integer such that d*(d+1)/2 >= n
 *      prev_tri = (d-1)*d/2
 *      pos  = n - prev_tri            (1‑based position on diagonal d)
 *      row  = d - pos + 1
 *      col  = pos
 *
 *  The value is p(row/col) if col divides row, otherwise 0. */
seq_t a(idx_t n)
{
    /* Find diagonal d:  d*(d+1)/2 >= n */
    idx_t d = 1;
    while ((d * (d + 1)) / 2 < n)
        ++d;

    idx_t prev_tri = (d - 1) * d / 2;
    idx_t pos      = n - prev_tri;
    idx_t row      = d - pos + 1;
    idx_t col      = pos;

    if (row % col != 0)
        return (seq_t)0;

    idx_t m = row / col;
    return get_partition(m);
}
