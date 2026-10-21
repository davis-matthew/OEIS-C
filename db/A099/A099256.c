/*
 * A099256  Expansion of g.f. (3-x)*(1+3*x+x^2)/((1-x-x^2)*(1+x-x^2)).
 * https://oeis.org/A099256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A099256
   Recurrence: a(0)=3, a(1)=8, a(2)=9, a(3)=23,
               a(n+4) = 3*a(n+2) - a(n)          (n ≥ 0). */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"

seq_t a(idx_t n)
{
    static const seq_t init[4] = { (seq_t)3, (seq_t)8, (seq_t)9, (seq_t)23 };

    /* Fast path for the first four terms. */
    if (n < 4) return init[(size_t)n];

    /* Allocate a temporary array large enough for indices 0 … n. */
    seq_t *buf = seq_alloc_new_array((size_t)n + 1);
    if (!buf) abort();                         /* allocation failure */

    /* Initialise the known terms. */
    for (size_t i = 0; i < 4; ++i) buf[i] = init[i];

    /* Compute successive terms using the recurrence. */
    for (size_t i = 4; i <= (size_t)n; ++i)
        buf[i] = (seq_t)3 * buf[i - 2] - buf[i - 4];

    return buf[(size_t)n];
}
