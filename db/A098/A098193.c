/*
 * A098193  Terms in a specific cycle of length 7 of the map x->A098189(x).
 * https://oeis.org/A098193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A098193
   Terms in a specific cycle of length 7 of the map x->A098189(x).
   The cycle members sorted ascending are:
   1608, 1748, 1920, 2028, 2088, 2584, 2776.  The sequence repeats these
   values periodically. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The cycle has length 7. */
    static const seq_t cycle[7] =
        { 1608, 1748, 1920, 2028, 2088, 2584, 2776 };

    /* Off‑by‑one indexing: a(1) is cycle[0]. */
    if (n == 0)   /* undefined for n<1; return zero for safety */
        return 0;

    idx_t i = (n - 1) % 7;
    return cycle[i];
}
