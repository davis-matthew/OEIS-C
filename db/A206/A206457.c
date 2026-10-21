/*
 * A206457  Number of 0..n arrays of length n+3 avoiding the consecutive
 *          pattern 0..n.
 * https://oeis.org/A206457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..floor((n+3)/(n+1))}
 *            ((-1)^i*(n+1)^((n+3)-(n+1)*i)*binomial((n+3)-n*i,i))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow, binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* The maximum i needed is floor((n+3)/(n+1)), which is 2 when n=1
       and 1 for all n>=2.  The loop is therefore extremely small.  */
    idx_t kmax = (n + 3) / (n + 1);
    seq_t base = (seq_t)(n + 1);
    seq_t result = 0;

    for (idx_t i = 0; i <= kmax; ++i)
    {
        /* exponent for (n+1)^((n+3)-(n+1)*i) */
        idx_t exp = (n + 3) - (n + 1) * i;
        seq_t pow = big_pow(base, (seq_t)exp);

        /* binomial coefficient binomial((n+3)-n*i, i) */
        idx_t n_minus_ui = (n + 3) - n * i;
        seq_t bin = binomial(n_minus_ui, i);

        /* term = (n+1)^exp * binomial(...) */
        seq_t term = seq_mul(pow, bin);

        if ((i & 1) == 0)
            result = result + term;   /* (+) sign */
        else
            result = result - term;   /* (-) sign */
    }

    return result;
}
