/*
 * A032249  "DHK[ 8 ]" (bracelet, identity, unlabeled, 8 parts) transform of
 *          1,1,1,1,...
 * https://oeis.org/A032249
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/16) * (binom(n-1,7) - 8*binom(floor(n/2),4) - (n even?
 *            (binom(n/2-1,3) - 8*binom(floor(n/4),2)) : 0) )
 * Range:     u8 none, u16 n <= 16, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* multiplicative binomial for small k (k <= 7) */
static inline seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 11) return 0;  /* offset is 11; behavior below does not matter */

    /* main terms */
    seq_t termA = binom_small(n - 1, 7);          /* C(n-1,7)   */
    seq_t termB = binom_small(n >> 1, 4);        /* C(floor(n/2),4) */
    seq_t sum = termA - (seq_t)8 * termB;

    /* extra term for even n */
    if ((n & 1) == 0)
    {
        seq_t termC = binom_small((n >> 1) - 1, 3); /* C(n/2-1,3) */
        seq_t termD = binom_small(n >> 2, 2);      /* C(floor(n/4),2) */
        sum = sum - termC + (seq_t)8 * termD;
    }

    return sum / (seq_t)16;
}
