/*
 * A133031  Triangle read by rows: row n lists divisors of n-th perfect number
 *          A000396(n).
 * https://oeis.org/A133031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 51, u32 n <= 93, u64 n <= 259,
 *            u128 n <= 445, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A133031
   Triangle read by rows: row n lists the divisors of the n‑th perfect number
   A000396(n) in ascending order.  The sequence is obtained by concatenating
   these rows.  For a perfect number with exponent p (i.e. P = 2^{p-1}(2^p‑1))
   the row contains the divisors
       2^0, 2^1, …, 2^{p-1}, (2^p‑1), 2(2^p‑1), …, 2^{p-1}(2^p‑1).
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Known exponents p of Mersenne primes giving perfect numbers.
       Only the first few are needed for n up to about ten thousand
       (the 16th perfect number has p = 2203).  The array is easily
       extended if larger values of n are required. */
    static const idx_t mersenne_p[] = {
        2u, 3u, 5u, 7u, 13u, 17u, 19u, 31u, 61u, 89u,
        107u, 127u, 521u, 607u, 1279u, 2203u, 2281u, 3217u,
        4253u, 4423u, 9689u, 9941u
    };
    const size_t np = sizeof(mersenne_p)/sizeof(mersenne_p[0]);

    idx_t cum = 0;                     /* cumulative number of terms processed */
    for (size_t idx = 0; idx < np; ++idx) {
        idx_t p   = mersenne_p[idx];    /* exponent for this perfect number */
        idx_t len = p * 2;              /* number of divisors in this row */

        if (n <= cum + len) {           /* this row contains the n-th term */
            idx_t pos = n - cum;        /* position within the row (1‑based) */

            if (pos <= p) {             /* pure powers of two */
                return ((seq_t)1) << (pos - 1);
            } else {                    /* multiples of the odd factor */
                seq_t odd = (((seq_t)1) << p) - 1; /* 2^p - 1 */
                idx_t exp = (pos - p) - 1;         /* shift amount */
                return odd << exp;
            }
        }
        cum += len;                      /* move to next row */
    }

    /* If the index is beyond the number of known perfect numbers, return 0. */
    return 0;
}
