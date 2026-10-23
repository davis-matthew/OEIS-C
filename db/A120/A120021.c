/*
 * A120021  Coefficients of x^n in the (n+1)-th self-composition of the g.f.
 *          of A120010: a(n) = [x^n] { (1-sqrt(1-4*x))/2 o x/(1-(n+1)*x) o
 *          (x-x^2) } for n>=1.
 * https://oeis.org/A120021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..n} Catalan(n-j) * [Sum_{i=1..j}
 *            (-1)^(j-i)*(n+1)^(i-1)* C(n-j+i, j-i)*C(n-j+i-1, i-1)] Where
 *            Catalan(k) = C(2k, k)/(k+1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   binom, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses helper routines from the utils directory:
   - catalan(idx_t k)      => returns Catalan(k)
   - binom(idx_t a, idx_t b) => returns C(a, b)
   These are included via the appropriate header files. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/catalan.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* OEIS offset starts at 1; return 0 for n == 0 (unused). */
    if (n == 0) return 0;

    idx_t np1 = n + 1;          /* n+1 as idx_t */

    seq_t pos = 0;              /* positive partial sums   */
    seq_t neg = 0;              /* negative partial sums   */
    seq_t curpow;               /* (n+1)^(i-1) in inner loop */
    seq_t term;

    for (idx_t j = 1; j <= n; ++j) {
        seq_t catal = catalan(n - j);   /* Catalan(n-j) */

        curpow = 1;                     /* (n+1)^(i-1) starts with i=1 */
        for (idx_t i = 1; i <= j; ++i) {
            /* m = n-j+i, k1 = j-i, k2 = i-1  */
            idx_t m   = n - j + i;
            idx_t k1  = j - i;
            idx_t k2  = i - 1;

            seq_t bin1 = binom(m, k1);          /* C(m, j-i)   */
            seq_t bin2 = binom(m - 1, k2);      /* C(m-1, i-1) */

            term = catal;
            term *= curpow;                     /* (n+1)^(i-1) */
            term *= bin1;
            term *= bin2;

            /* Sign: (-1)^(j-i) */
            if (k1 & 1)   /* odd exponent -> negative */
                neg += term;
            else
                pos += term;

            curpow *= (seq_t)np1;              /* next power of (n+1) */
        }
    }

    return pos - neg;   /* pos >= neg for all n >= 1 */
}
