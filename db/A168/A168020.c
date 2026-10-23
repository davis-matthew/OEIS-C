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
 * Helpers:   get_partition
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
#include "../utils/get_partition.h"

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
