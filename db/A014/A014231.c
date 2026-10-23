/*
 * A014231  (Product of 3 successive Catalan numbers)/2.
 * https://oeis.org/A014231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Catalan(n) * Catalan(n+1) * Catalan(n+2)) / 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation divides one of the three factors by 2 first
   to avoid intermediate overflow on fixed‑width integer types. */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t c0 = catalan(n);
    seq_t c1 = catalan(n + 1);
    seq_t c2 = catalan(n + 2);
    seq_t res;

    if ((c0 & 1) == 0)          /* c0 is even; divide it by 2 first */
    {
        res = (c0 / 2) * c1;
        res = res * c2;
    }
    else if ((c1 & 1) == 0)     /* c1 is even */
    {
        res = c0 * (c1 / 2);
        res = res * c2;
    }
    else                        /* c2 must be even */
    {
        res = c0 * c1;
        res = res * (c2 / 2);
    }

    return res;
}
