/*
 * A189889  Maximum number of nonattacking kings on an n X n toroidal board.
 * https://oeis.org/A189889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A189889: maximum number of nonattacking kings on an n × n toroidal board
   Formula: a(1) = 1
            a(n) = ⌊( n * ⌊n/2⌋ ) / 2⌋   for n > 1   (Watkins and Ricci, 2004) */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    /* compute ⌊n/2⌋ using integer division */
    idx_t half = n / 2;
    /* product fits in idx_t for reasonable n (e.g., n ≤ 10⁴ → product < 5·10⁷) */
    idx_t prod = n * half;
    idx_t result = prod / 2;
    return (seq_t)result;
}
