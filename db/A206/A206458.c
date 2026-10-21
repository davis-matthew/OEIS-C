/*
 * A206458  Number of 0..n arrays of length n+4 avoiding the consecutive
 *          pattern 0..n.
 * https://oeis.org/A206458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=0..floor((n+4)/(n+1))} ((-1)^i *
 *            (n+1)^{(n+4)-(n+1)i} * binomial((n+4)-n i, i))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* The following utilities are available from the build environment. */
#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) */
#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; safety for n==0 */

    /* Determine the maximum i in the sum: floor((n+4)/(n+1)) */
    idx_t max_i = (n + 4) / (n + 1);

    seq_t sum = 0;

    for (idx_t i = 0; i <= max_i; ++i) {
        /* a = (n+4) - n*i  for the binomial coefficient */
        idx_t a_val = (n + 4) - n * i;
        seq_t bin = binomial(a_val, i);

        /* exponent for (n+1)^ ...  */
        idx_t exp_val = (n + 4) - (n + 1) * i;
        seq_t base_pow = big_pow((seq_t)(n + 1), (seq_t)exp_val);

        /* term = bin * base_pow */
        seq_t term = bin * base_pow;

        /* Add or subtract based on parity of i */
        if (i & 1) {
            sum -= term;          /* safe because term <= sum at this point */
        } else {
            sum += term;
        }
    }

    return sum;
}
