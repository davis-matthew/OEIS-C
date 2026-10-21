/*
 * A068705  a(n) is the concatenation of n, (n-1)^2, (n-2)^3, (n-3)^4, ...,
 *          2^(n-1) and 1.
 * https://oeis.org/A068705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concatenate_{k=0}^{n-1} (n-k)^(k+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   big_pow, digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; for n==0 we simply return 0. */
    if (n == 0) return 0;

    seq_t result = (seq_t)n;  /* first term: n^1 */

    for (idx_t k = 1; k < n; ++k) {
        seq_t base   = (seq_t)(n - k);         /* (n-k) */
        seq_t exp    = (seq_t)(k + 1);         /* exponent k+1 */
        seq_t term   = big_pow(base, exp);     /* (n-k)^(k+1)  */
        idx_t  digs   = (idx_t)digit_len(term);/* decimal digits of term */
        seq_t mult   = pow10(digs);            /* 10^digs          */
        result = result * mult + term;         /* append term       */
    }
    return result;
}
