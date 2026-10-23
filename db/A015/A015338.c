/*
 * A015338  Gaussian binomial coefficient [ n,7 ] for q = -2.
 * https://oeis.org/A015338
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Gaussian binomial coefficient [n,7] for q = -2 * a(n) =
 *            product_{i=0}^{6} ((-2)^(n-i) - 1) / * product_{j=1}^{7} ((-2)^j
 *            - 1) * Offset: n ≥ 7 * * Requirements: * - Seq_t can be
 *            8,16,32,64,128 bits or arbitrary precision (C++17). * - Uses
 *            only integer arithmetic, a few utilities (gcd, big_p...
 * Range:     i8 none, i16 none, i32 n <= 11, i64 n <= 16, i128 n <= 25,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/big_pow.h"

/* Return the Gaussian binomial coefficient [n,7] for q = -2.
 * For n < 7 the behaviour is undefined; this implementation reports 0. */
seq_t a(idx_t n)
{
    if (n < 7) return 0;          /* offset enforcement */

    /* --------------------------------------------------------------------
       Denominator: product_{j=1}^{7} ((-2)^j - 1)
       This constant is independent of n; we compute it once.
       -------------------------------------------------------------------- */
    static seq_t denom = 0;
    static bool denom_initialized = false;

    if (!denom_initialized) {
        seq_t base = (seq_t)-2;
        seq_t product = 1;
        for (int j = 1; j <= 7; ++j) {
            seq_t term = big_pow(base, (seq_t)j) - 1; /* (q^j - 1) */
            product *= term;
        }
        /* The product is positive (see analysis). */
        denom = product;
        if (denom < 0) denom = -denom;
        denom_initialized = true;
    }

    /* --------------------------------------------------------------------
       Compute the numerator and cancel factors with the denominator
       on the fly to keep intermediate values manageable.
       -------------------------------------------------------------------- */
    seq_t result = 1;
    seq_t denom_rem = denom;        /* remaining denominator part */

    for (int i = 0; i < 7; ++i) {
        idx_t exp = n - (idx_t)i;            /* current exponent n-i */
        seq_t pow_val = big_pow((seq_t)-2, (seq_t)exp);  /* (-2)^(n-i) */
        seq_t num_term = pow_val - 1;                 /* numerator factor */

        seq_t g = gcd(num_term, denom_rem);
        if (g != 1) {
            num_term /= g;
            denom_rem /= g;
        }
        result *= num_term;
    }
    /* At this point denom_rem should be 1; any remainder would be an error. */
    return result;
}
