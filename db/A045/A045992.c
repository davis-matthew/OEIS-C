/*
 * A045992  a(n) = binomial(2n,n) - n; number of (weakly) increasing or
 *          decreasing maps from 1,...,n to 1,...,n.
 * https://oeis.org/A045992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n, n) - n
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute binomial multiplicatively, reducing by gcd each step
   to avoid overflow for the 64‑bit build while staying correct
   for arbitrary‑precision builds.  */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/*  Computes a(n) = binomial(2*n, n) - n  */
seq_t a(idx_t n)
{
    /* n converted to seq_t */
    seq_t n_seq = (seq_t)n;
    seq_t res = 1;

    /* multiplicative binomial computation:
       res = product_{k=1}^{n} (n + k) / k  */
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t num = n_seq + (seq_t)k;  /* numerator = n + k  */
        seq_t den = (seq_t)k;          /* denominator = k   */

        /* first simplify num/den */
        seq_t g = gcd(num, den);
        if (g != 0) {
            num /= g;
            den /= g;
        }

        /* further reduce with current result */
        if (den != 1) {
            seq_t g2 = gcd(res, den);
            if (g2 != 0) {
                res /= g2;
                den /= g2;
            }
        }

        /* at this point den should be 1 */
        /* multiply */
        res *= num;
    }

    return res - n_seq;
}
