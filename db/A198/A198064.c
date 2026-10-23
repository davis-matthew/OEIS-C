/*
 * A198064  Triangle read by rows (n >= 0, 0 <= k <= n, m = 4); T(n,k) =
 *          Sum{j=0..m} Sum{i=0..m} (-1)^(j+i)*C(i,j)*n^j*k^(m-j).
 * https://oeis.org/A198064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where T(r,c) = c⁴ – 2·c³·r + 4·c²·r² – 3·c·r³ +
 *            r⁴, and the sequence is read row‑wise over the triangle for m =
 *            4
 * Range:     u8 n <= 9, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    long double d = sqrt((long double)8 * n + 1);
    idx_t r = (idx_t)((d - 1) / 2);

    /* Correct possible rounding errors from the sqrt */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    while (r * (r + 1) / 2 > n)
        --r;

    /* Column c within the row */
    idx_t c = n - r * (r + 1) / 2;

    /* Convert to seq_t for computation */
    seq_t rn = (seq_t)r;
    seq_t cn = (seq_t)c;

    /* Evaluate the polynomial: c⁴ – 2·c³·r + 4·c²·r² – 3·c·r³ + r⁴ */
    seq_t val =
        cn * cn * cn * cn
      - (seq_t)2 * cn * cn * cn * rn
      + (seq_t)4 * cn * cn * rn * rn
      - (seq_t)3 * cn * rn * rn * rn
      + rn * rn * rn * rn;

    return val;
}
