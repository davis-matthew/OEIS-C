/*
 * A134015  Expansion of (1 - phi(-q) * phi(q^4)) / 2 in powers of q where
 *          phi() is a Ramanujan theta function.
 * https://oeis.org/A134015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A134015
   Multiplicative definition:
     a(1)    = 1
     a(2)    = 0
     a(2^e)  = -2   (e>1)
     a(p^e)  = e+1          if p ≡ 1 (mod 4)
     a(p^e)  = (1+(-1)^e)/2 if p ≡ 3 (mod 4)
   For composite n the value is the product of its prime power
   contributions. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1, n≥1 in OEIS */

    uint64_t m = (uint64_t)n;
    seq_t result = 1;

    /* factorise m by trial division – fast enough for n≤10000 */
    while (m > 1) {
        uint64_t p, e = 0;

        if ((m & 1ULL) == 0) {  /* factor 2 */
            p = 2ULL;
            while ((m & 1ULL) == 0) { m >>= 1; ++e; }
        } else {
            p = 3ULL;
            for (; p * p <= m; p += 2ULL) {
                if (m % p == 0) break;
            }
            if (p * p > m) p = m;  /* remaining prime factor */
            while (m % p == 0) { m /= p; ++e; }
        }

        /* compute the contribution of this prime power */
        seq_t term;
        if (p == 2ULL) {
            if (e == 1ULL) return 0;      /* a(2) = 0 */
            term = (seq_t)-2;             /* a(2^e) = -2 for e>1 */
        } else if ((p & 3ULL) == 1ULL) {  /* p ≡ 1 (mod 4) */
            term = (seq_t)(e + 1ULL);
        } else { /* p ≡ 3 (mod 4) */
            if (e & 1ULL) return 0;       /* odd exponent yields 0 */
            term = 1;                     /* even exponent yields 1 */
        }

        result *= term;
    }
    return result;
}
