/*
 * A076175  Denominator of Sum_{i+j+k=n, i,j,k>=1} (i*j)/k.
 * https://oeis.org/A076175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator of sum_{i+j+k=n, i,j,k>=1} (i*j)/k
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This implementation evaluates the sum by iterating over k = 1 .. n-2
   with the term (n-k)(n-k-1)(n-k+1)/(6k).  The sum is accumulated as a
   rational number, each step reducing the fraction to keep the numbers
   small.  The final denominator after the reduction is returned.
*/

#include "../utils/gcd.h"

typedef struct {
    seq_t num;   /* numerator (non‑negative) */
    seq_t den;   /* denominator (positive)   */
} Rat;

/* Reduce a rational number to lowest terms */
static Rat simplify(Rat r)
{
    if (r.num == 0) {          /* 0/x is 0/1 */
        r.den = 1;
        return r;
    }
    seq_t g = gcd(r.num, r.den);
    r.num /= g;
    r.den /= g;
    return r;
}

/* Add two rational numbers: a + b */
static Rat add_rat(const Rat a, const Rat b)
{
    Rat r;
    r.num = a.num * b.den + b.num * a.den;
    r.den = a.den * b.den;
    return simplify(r);
}

/* Return the denominator of the sequence a(n) */
seq_t a(idx_t n)
{
    if (n < 3) {
        return 1;   /* No valid triples exist; value is 0/1 */
    }

    Rat sum = {0, 1};   /* Initial sum = 0 */

    for (idx_t k = 1; k <= n - 2; ++k) {
        seq_t t = n - k;                           /* t = n - k */
        seq_t num = t * (t - 1) * (t + 1);         /* (n-k)(n-k-1)(n-k+1) */
        seq_t den = 6 * k;                         /* 6k */

        /* Reduce the current fraction before adding */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        Rat term = {num, den};
        sum = add_rat(sum, term);
    }

    return sum.den;
}
