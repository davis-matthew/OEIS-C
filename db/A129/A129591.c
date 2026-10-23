/*
 * A129591  For each permutation p of {1,2,...,n} define min(p) = min{ p(i) +
 *          i : i = 1..n }; a(n) is the sum of min(p) of all p.
 * https://oeis.org/A129591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} (n-k+1)*k!*((k+1)^(n-k)-k^(n-k))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t res  = 0;      /* accumulator for the result            */
    seq_t fact = 1;      /* k!  (starts with 0! = 1)              */

    for (idx_t k = 0; k < n; ++k) {
        idx_t exp = n - k;                       /* exponent n-k  */

        /* powers (k+1)^(n-k) and k^(n-k) */
        seq_t pow1 = big_pow((seq_t)(k + 1), (seq_t)exp);
        seq_t pow2 = big_pow((seq_t)k, (seq_t)exp);

        /* difference of the two powers */
        seq_t diff = pow1 - pow2;

        /* multiplier (n-k+1) */
        seq_t mult = (seq_t)(n - k + 1);

        /* term = (n-k+1) * k! * diff */
        seq_t term = fact * mult;
        term = term * diff;

        /* add to the running sum */
        res += term;

        /* update factorial for next k ( (k+1)! ) */
        fact *= (k + 1);
    }
    return res;
}
