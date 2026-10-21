/*
 * A093447  Triangle a(n,k) read by rows n which contain columns k=1,2,..,n,
 *          where each entry is the product of numbers (k-1)*n-T(k-2)+1
 *          through k*n-T(k-1).
 * https://oeis.org/A093447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the product of consecutive integers forming the n-th
 *            element of the triangle defined by A093447. For the n-th entry
 *            we find its row r and column k, then compute low = (k-1)*r -
 *            T(k-2) + 1 high = k*r - T(k-1) where T(m) = m*(m+1)/2 (and
 *            T(-1)=0). The value is the product of all integers fr...
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   triangle_T
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required integer types are defined by the build environment:
   idx_t  – unsigned integer type for indices (e.g. uint64_t)
   seq_t  – unsigned integer type for sequence values   */

#include <math.h>   /* for sqrt() */
#include "../utils/triangle_T.h"

seq_t a(idx_t n)
{
    /* n >= 1 by problem statement; return 0 for n==0 */
    if (n == 0)
        return 0;

    /* ----- find row r and column c ----- */
    /* r is the smallest integer with r*(r+1)/2 >= n   */
    double nd = (double)n;
    idx_t r = (idx_t)((sqrt(8.0 * nd + 1.0) - 1.0) / 2.0);
    if (((seq_t)r * (r + 1) / 2) < (seq_t)n)
        ++r;

    idx_t c = n - r * (r - 1) / 2;  /* column in this row (1 … r) */

    /* ----- compute bounds for the product ----- */
    seq_t Tk2 = 0; /* T(k-2); zero when k==1 */
    if (c >= 2) {
        idx_t m = c - 2;
        Tk2 = triangle_T(m);
    }
    seq_t Tk1 = triangle_T(c - 1);

    seq_t low  = ((seq_t)(c - 1)) * r - Tk2 + 1;
    seq_t high = ((seq_t)c) * r - Tk1;

    /* ----- multiply the consecutive integers ----- */
    seq_t prod = 1;
    for (seq_t v = low; v <= high; ++v)
        prod *= v;

    return prod;
}
