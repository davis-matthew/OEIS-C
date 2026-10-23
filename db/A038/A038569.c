/*
 * A038569  Denominators in a certain bijection from positive integers to
 *          positive rationals.
 * https://oeis.org/A038569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, core, nice
 * Formula:   not stated
 * Range:     u8 n <= 9960, u16 n <= 9960, u32 n <= 9960, u64 n <= 9960,
 *            u128 n <= 9960, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A038569.
   The sequence is obtained by enumerating all reduced fractions
   0 < p/q ≤ 1 sorted by increasing denominator and then
   increasing numerator.  Each fraction is written once, and
   for every fraction except the first one its reciprocal is
   written immediately afterwards.
   The function returns the denominator of the n‑th term
   (a(0)=1).  For n≥1 let m=(n+1)/2; the n‑th term corresponds
   to the (m‑th) fraction in the reduced list.
   If n is odd the term is the fraction itself – return its
   denominator.  If n is even the term is the reciprocal – return
   its numerator.  For n=0 return 1. */

#include <stdint.h>

/* Euclidean GCD for idx_t (unsigned 64‑bit). */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Find the m‑th reduced fraction p/q (0‑based) with p/q ≤ 1.
   For m=0 the first fraction is 1/1. */
static void get_fraction(idx_t m, idx_t *p, idx_t *q)
{
    if (m == 0) {
        *p = 1;
        *q = 1;
        return;
    }

    idx_t count = 1;          /* fractions already seen after 1/1 */
    idx_t denom = 2;          /* start with denominator 2 */

    for (;; ++denom) {
        for (idx_t num = 1; num < denom; ++num) {
            if (gcd_idx(num, denom) == 1) {
                if (count == m) {
                    *p = num;
                    *q = denom;
                    return;
                }
                ++count;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;               /* a(0)=1 */

    idx_t m = (n + 1) / 2;             /* position in reduced list */
    idx_t p, q;
    get_fraction(m, &p, &q);

    if (n % 2 == 1)   /* original fraction */
        return (seq_t)q;   /* denominator */
    else              /* reciprocal */
        return (seq_t)p;   /* numerator */
}
