/*
 * A008645  Molien series of 6 X 6 upper triangular matrices over GF( 2 ).
 * https://oeis.org/A008645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the coefficient of x^n in
 *            1/((1-x)(1-x^2)(1-x^4)(1-x^8)(1-x^16)(1-x^32)); equivalently,
 *            the number of partitions of n into powers of 2 not exceeding 32
 * Range:     u8 n <= 35, u16 n <= 161, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Each call recomputes the sequence up to n using the standard
   coin‑change DP (order–independent).  The cost is O(n·6) operations,
   well within the required limits for n ≤ 10 000. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Coin values: powers of two up to 2^5 = 32 */
    static const idx_t coins[] = {1, 2, 4, 8, 16, 32};
    static const size_t num_coins = sizeof(coins) / sizeof(coins[0]);

    /* Allocate array for current computation */
    size_t len = (size_t)(n + 1);
    seq_t *ways = (seq_t *)malloc(len * sizeof(seq_t));
    if (!ways)  /* allocation failed – return 0 as a safe fallback */
        return (seq_t)0;

    /* Initialise */
    ways[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        ways[i] = (seq_t)0;

    /* Coin change DP: for each coin, update all positions >= coin */
    for (size_t ci = 0; ci < num_coins; ++ci) {
        idx_t c = coins[ci];
        for (idx_t i = c; i <= n; ++i) {
            ways[i] += ways[i - c];
        }
    }

    seq_t result = ways[n];
    free(ways);
    return result;
}
