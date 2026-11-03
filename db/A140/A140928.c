/*
 * A140928  Number of 5 X 10 matrices with elements in 0..n with each row and
 *          each column in nondecreasing order. 5,10,n can be permuted, see
 *          formula.
 * https://oeis.org/A140928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{i=0}^{r-1} ( binomial(s, p+i) * i! / (s-i)^(r-i-1) )
 *            with p = n, q = 10, r = 5 and s = p + q + r - 1 = n + 14
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-01)
 * Helpers:   binomial, gcd, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"
#include "../utils/ipow_i32.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

/* Compute the OEIS sequence A140928: number of 5×10 matrices with
   elements in 0..n and nondecreasing rows and columns. */
seq_t a(idx_t n)
{
    const idx_t q = 10;          /* second dimension */
    const idx_t r = 5;           /* first dimension  */
    idx_t p = n;                 /* primary parameter   */
    idx_t s = p + q + r - 1;     /* n + 14              */

    seq_t num = 1;  /* numerator of the running product   */
    seq_t den = 1;  /* denominator of the running product */

    for (idx_t i = 0; i < r; ++i) {
        /* binomial(s, p+i) and i! */
        seq_t term_num = binomial(s, p + i) * factorial(i);

        /* denominator (s-i)^(r-i-1) */
        seq_t term_den = ipow_i32((seq_t)(s - i), (int)(r - i - 1));

        /* multiply into the running product            */
        num *= term_num;
        den *= term_den;

        /* cancel common factors to keep numbers small */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;
    }

    /* After cancellation den should be 1 */
    return num;
}
