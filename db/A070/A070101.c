/*
 * A070101  Number of obtuse integer triangles with perimeter n.
 * https://oeis.org/A070101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of obtuse integer triangles with perimeter n
 * Range:     u8 n <= 32, u16 n <= 517, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_obtuse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Count integer triples (a,b,c) with
       1 <= a <= b <= c,
       a + b + c = n,
       a + b > c,    (triangle inequality) 
       c^2 > a^2 + b^2  (obtuse)
*/
#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_obtuse.h"

/* External entry point requested by the benchmark harness. */
seq_t a(idx_t n)
{
    /* For n <= 0 the sequence is undefined; return 0. */
    if (n == 0) return 0;

    return count_obtuse(n);
}
