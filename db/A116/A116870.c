/*
 * A116870  Seventh column of triangle A028364.
 * https://oeis.org/A116870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..6} Catalan(k) * Catalan(n+6-k)
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The small Catalan numbers needed in the summation.  They are constant
   values 1,1,2,5,14,42,132.  They are stored once in a static array
   of type seq_t. */
static const seq_t SMALL_CATS[7] = { 1, 1, 2, 5, 14, 42, 132 };

/* Include the helper that computes Catalan numbers.  The helper accepts
   an idx_t argument and returns a seq_t. */
#include "../utils/catalan.h"

/* Compute a(n) for any non‑negative n.  The function uses at most seven
   calls to the provided catalan helper and a single 64‑bit addition loop
   (if seq_t is 64‑bit) or an arbitrary‑precision addition loop.  The
   algorithm is independent of the order of calls and memoises
   internally through the catalan helper (if it has that feature). */
seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    idx_t base = n + 6;          /* the largest index needed */
    for (idx_t k = 0; k <= 6; ++k) {
        sum += SMALL_CATS[(size_t)k] * catalan(base - k);
    }
    return sum;
}
