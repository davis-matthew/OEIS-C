/*
 * A070101  Number of obtuse integer triangles with perimeter n.
 * https://oeis.org/A070101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of obtuse integer triangles with perimeter n
 * Range:     u8 n <= 32, u16 n <= 517, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 85 terms through n = 969 (2026-11-01)
 * Helpers:   none
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

/* Count obtuse integer triangles for a fixed perimeter n. */
static seq_t count_obtuse(idx_t n)
{
    seq_t count = 0;

    /* a ranges from 1 to floor(n/3). */
    for (idx_t a = 1; a <= n / 3; ++a)
    {
        /* b ranges from a to floor((n-a)/2). */
        idx_t b_max = (n - a) / 2;
        for (idx_t b = a; b <= b_max; ++b)
        {
            idx_t c = n - a - b;
            /* Ensure c >= b (already satisfied by b <= (n-a)/2). */
            if (c < b) break;

            /* Triangle inequality: a + b > c. */
            if (a + b <= c) continue;

            /* Check obtuse condition: c^2 > a^2 + b^2. */
            seq_t left  = (seq_t)c * c;
            seq_t right = (seq_t)a * a + (seq_t)b * b;
            if (left > right) ++count;
        }
    }
    return count;
}

/* External entry point requested by the benchmark harness. */
seq_t a(idx_t n)
{
    /* For n <= 0 the sequence is undefined; return 0. */
    if (n == 0) return 0;

    return count_obtuse(n);
}
