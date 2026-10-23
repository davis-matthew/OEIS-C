/*
 * A119328  Number triangle T(n,k)=sum{i=0..n, (-1)^(n-i)*C(n,i)*sum{j=0..i-k,
 *          C(k,2j)*C(i-k,2j)}}.
 * https://oeis.org/A119328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 48, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = T(m,k) where T(n,k) = Σ_{i=0..n} (-1)^(n-i) * C(n,i) *
 *          Σ_{j=0..i-k} C(k,2j) * C(i-k,2j)
 *  (triangle entries are stored row-major: n=0 has one entry,
 *   n=1 has two entries, etc.)
 *
 * The code uses the equivalent expression
 *   T(n,k) = Σ_{j=0..⌊k/2⌋, 2j≤n-k} C(k,2j) * (-1)^{n‑k‑2j}
 *            * C(k+(n‑k‑2j)-1, n‑k‑2j)
 * obtained from the generating‑function of each column.
 */

#include <stddef.h>
#include <math.h>
#include "../utils/binom.h"   /* binom(idx_t a, idx_t b) returns seq_t */

seq_t a(idx_t idx)
{
    /* Determine (n,k) for the linear index idx. */
    if (idx == 0)  /* special case at the very beginning */
        return binom(0, 0);   /* T(0,0) = 1 */

    /* Solve (n+1)(n+2)/2 > idx for n. */
    long double tmp = sqrtl(8.0L * idx + 1.0L);
    idx_t n = (idx_t)((tmp - 1.0L) / 2.0L);

    /* Correct possible rounding errors. */
    while ((n + 1) * (n + 2) / 2 <= idx) ++n;
    while (n * (n + 1) / 2 > idx) --n;

    idx_t start = n * (n + 1) / 2;   /* first index of row n */
    idx_t k = idx - start;           /* column within the row */

    /* Handle the exceptional value T(0,0) explicitly. */
    if (n == 0 && k == 0) return binom(0, 0);

    seq_t result = 0;
    /* Sum over j where 2j <= k and n-k-2j >= 0. */
    for (idx_t j = 0; j <= k; ++j)
    {
        idx_t twoj = j << 1;
        if (twoj > k) break;
        idx_t s = n - k - twoj;
        if (s > n) continue;          /* s cannot exceed n, safety */
        if (s >= 0)
        {
            seq_t t1 = binom(k, twoj);               /* C(k,2j)           */
            seq_t t2 = binom(k + s - 1, s);           /* C(k+s-1, s)        */
            seq_t term = t1 * t2;
            if (s & 1) term = -term;                 /* (-1)^s factor      */
            result += term;
        }
    }

    return result;
}
