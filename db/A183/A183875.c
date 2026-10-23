/*
 * A183875  Triangle T(n,k) for A(x)^k=sum(n>=k T(n,k)*x^n), where o.g.f. A(x)
 *          satisfies A(x)=(a+b*x*A(x))/(c-d*x*A(x)), a=1,b=2,c=1,d=2.
 * https://oeis.org/A183875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binom, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A183875: triangle T(n,k) with a=1,b=2,c=1,d=2, read row‑wise.
   T(n,k) = k/n * 2^(n-k) * Σ_{i=0}^{n-k} C(n,n-k-i) * C(i+n-1,n-1)
   a(n) = T(r,c) where r is the smallest integer with r(r+1)/2 ≥ n
          and c = n - r(r-1)/2.
   Complexity: O(r^2) per call (r≈√(2n)), well below the limits for n≤10000.
*/

#include "../utils/binom.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Find the row r that contains the n‑th entry. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* Determine the column c within that row. */
    idx_t before = (r - 1) * r / 2;   /* elements before row r */
    idx_t c      = n - before;        /* 1‑based column index, 1 ≤ c ≤ r */
    idx_t rest   = r - c;             /* n - k in the formula */

    /* Compute the summation part Σ C(r, r-k-i) * C(i+r-1,r-1) */
    seq_t sum = 0;
    for (idx_t i = 0; i <= rest; ++i) {
        seq_t a1 = binom(r, rest - i);            /* C(r, r-k-i) */
        seq_t a2 = binom(i + r - 1, r - 1);        /* C(i+r-1,r-1) */
        sum += a1 * a2;
    }

    /* Multiply by 2^(rest) and by k, then divide by n. */
    seq_t pow2 = pow_uint(2, rest);
    seq_t result = sum * pow2;                     /* Σ * 2^(n-k) */
    result *= (seq_t)c;                             /* * k */
    result /= (seq_t)r;                             /* / n */

    return result;
}
