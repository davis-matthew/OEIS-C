/*
 * A036361  Number of labeled 2-trees with n nodes.
 * https://oeis.org/A036361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = binom(n,2) * (2*n-3)^(n-4) (for n >= 4)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* with a(1)=0, a(2)=1, a(3)=1 */

#include <stdint.h>
#include <stdbool.h>

/* The helper functions binomial and big_pow are provided by the build. */
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)           return (seq_t)0;
    if (n == 1)           return (seq_t)0;
    if (n == 2)           return (seq_t)1;
    if (n == 3)           return (seq_t)1;

    /* Compute binomial(n,2) */
    seq_t bin = binomial(n, (idx_t)2);

    /* Base of the power: 2*n - 3 */
    seq_t base = (seq_t)((seq_t)2 * (seq_t)n - (seq_t)3);

    /* Exponent: n - 4 (non‑negative for n >= 4) */
    seq_t exp = (seq_t)(n - 4);

    /* Compute (2*n-3)^(n-4) using the provided big_pow helper */
    seq_t powv = big_pow(base, exp);

    /* Final result */
    return bin * powv;
}
