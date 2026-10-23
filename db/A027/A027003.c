/*
 * A027003  a(n) = A026998(2*n, n+4).
 * https://oeis.org/A027003
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(2*n+9) - (48*n^7 - 280*n^6 + 1596*n^5 - 910*n^4 +
 *            10122*n^3 + 20405*n^2 + 46509*n + 47880)/630 (Lucas is Lucas
 *            sequence L(0)=2, L(1)=1, L(n)=L(n-1)+L(n-2))
 * Range:     u8 none, u16 none, u32 n <= 14, u64 n <= 41, u128 n <= 87,
 *            big any n
 * Helpers:   poly_f
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/poly_f.h"
static seq_t lucas(idx_t k)
{
    if (k == 0) return (seq_t)2;
    if (k == 1) return (seq_t)1;
    seq_t a = (seq_t)2;
    seq_t b = (seq_t)1;
    for (idx_t i = 2; i <= k; ++i)
    {
        seq_t tmp = a + b;
        a = b;
        b = tmp;
    }
    return b;
}

seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;      /* values before the offset are undefined */
    idx_t k = 2 * n + 9;             /* index for Lucas */
    seq_t luc = lucas(k);
    seq_t f   = poly_f(n);
    return luc - f;                  /* luc - f is guaranteed positive for n ≥ 4 */
}
