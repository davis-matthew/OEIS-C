/*
 * A204431  Symmetric matrix: f(i,j)=(2*(i+j)+1 mod 3), by antidiagonals.
 * https://oeis.org/A204431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2*(ceil((sqrt(8*n+1)+1)/2)) + 1) mod 3 (equivalently,
 *            let s be the smallest integer with s*(s-1)/2 ≥ n, then a(n) =
 *            (2*s + 1) mod 3)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* n is 1‑based index */
    unsigned long long u = (unsigned long long) n;          /* work in unsigned long long */
    /* approximate value of m = ceil( (sqrt(8u+1)-1)/2 )  */
    double d = sqrt(1.0 + 8.0 * (double)u);
    unsigned long long m = (unsigned long long)((d - 1.0) / 2.0);

    /* correct possible off‑by‑one errors from rounding */
    while ((m * (m + 1)) / 2 < u) ++m;
    while (m > 0 && ((m - 1) * m) / 2 >= u) --m;

    /* diagonal number is s = m + 1 */
    unsigned long long s_mod3 = (m + 1) % 3;
    seq_t val;
    if (s_mod3 == 0)          val = (seq_t)1;
    else if (s_mod3 == 1)     val = (seq_t)0;
    else                      val = (seq_t)2;
    return val;
}
