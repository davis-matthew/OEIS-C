/*
 * A073820  a(n) = n+1-i such that prime(i)*(n+1-i) is maximized (1 <= i <=
 *          n).
 * https://oeis.org/A073820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n+1-i such that prime(i)*(n+1-i) is maximized (1 <= i <=
 *            n)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1, return 0 */

    /* Cache the first n primes across calls. */
    static uint32_t *primes = NULL;
    static size_t primes_cnt = 0;

    if ((size_t)n > primes_cnt) {
        free(primes);                     /* free old cache if present */
        sieve_first_n_primes(n, &primes, &primes_cnt);
    }

    uint64_t max_prod = 0;
    idx_t best_j = 1;                      /* default for n == 1 */

    /* Find i maximizing prime(i) * (n+1-i). */
    for (idx_t i = 1; i <= n; ++i) {
        uint64_t prod = (uint64_t)primes[i - 1] * (uint64_t)(n + 1 - i);
        if (prod > max_prod) {
            max_prod = prod;
            best_j = n + 1 - i;
        }
    }

    return (seq_t)best_j;
}
