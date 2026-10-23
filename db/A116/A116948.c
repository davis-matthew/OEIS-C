/*
 * A116948  Riordan array ((1+2x^2)/(1-x^3),x).
 * https://oeis.org/A116948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 1 if (r‑c) mod 3 = 0, 2 if (r‑c) mod 3 = 2, 0 otherwise,
 *            where r = ⌊(√(8n+1)−1)/2⌋ and c = n – r(r+1)/2. This produces
 *            the flattened rows of the Riordan array ((1+2x²)/(1‑x³),x)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>     /* for sqrt */

seq_t a(idx_t n)
{
    /* Determine the row r such that r(r+1)/2 > n >= (r−1)r/2 */
    idx_t r = (idx_t)((sqrt((double)8 * n + 1) - 1) / 2);

    /* First index of this row */
    idx_t first = r * (r + 1) / 2;

    /* Column index within the row */
    idx_t c = n - first;

    /* (r - c) is non‑negative */
    idx_t delta = r - c;

    int rem = (int)(delta % 3);
    if (rem == 0) return (seq_t)1;
    if (rem == 2) return (seq_t)2;
    return (seq_t)0;
}
