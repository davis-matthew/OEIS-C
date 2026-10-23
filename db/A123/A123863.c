/*
 * A123863  Expansion of (c(q^3) - c(q^6) - 2*c(q^12)) / 3 in powers of q
 *          where c(q) is a cubic AGM theta function.
 * https://oeis.org/A123863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is multiplicative. For each prime power p^e dividing n, the
 *            contribution is -1 if p == 2 0 if p == 3 e + 1 if p % 6 == 1 (1
 *            + (-1)^e)/2 if p % 6 == 5 and the value is the product of the
 *            contributions. In particular any factor 3 makes a(n) = 0. This
 *            implements OEIS A123863
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* the build supplies the types seq_t and idx_t; common headers are
   always available.  */
#include <stdint.h>

/* external linkage, exactly one function as required. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined below offset */
    if (n == 1) return 1;         /* a(1) = 1 */

    seq_t res = 1;                /* multiplicative accumulator */
    idx_t m = n;                  /* remaining factor to process */

    /* trial division up to sqrt(m). Use p <= m/p to avoid overflow. */
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p != 0) continue;                 /* not a factor */

        /* count exponent e of prime p */
        idx_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }

        /* compute contribution */
        if (p == 2) {
            res *= (seq_t)-1;                      /* always -1 */
        } else if (p == 3) {
            return 0;                              /* any 3 => 0 */
        } else if (p % 6 == 1) {
            res *= (seq_t)(e + 1);
        } else { /* p % 6 == 5 */
            /* (1+(-1)^e)/2 is 1 if e even, 0 if e odd */
            res *= ((e & 1) == 0) ? (seq_t)1 : (seq_t)0;
        }

        if (res == 0) return 0;                    /* zero propagates */
    }

    /* if a larger prime > sqrt(n) remains */
    if (m > 1) {
        idx_t p = m;                /* exponent e = 1 */
        if (p == 2) {
            res *= (seq_t)-1;
        } else if (p == 3) {
            return 0;
        } else if (p % 6 == 1) {
            res *= (seq_t)2;        /* e+1 = 2 */
        } else {                    /* p % 6 == 5, e = 1 odd */
            res *= (seq_t)0;
        }
    }

    return res;
}
