/*
 * A236214  Sum of the eighth powers of the first n primes.
 * https://oeis.org/A236214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} prime(k)^8
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 47, u128 n <= 1000,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* not needed for offset 1, but harmless */

    uint32_t *primes = NULL;
    size_t cnt = 0;
    sieve_first_n_primes(n, &primes, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t p2 = p * p;        /* p^2 */
        seq_t p4 = p2 * p2;      /* p^4 */
        seq_t p8 = p4 * p4;      /* p^8 */
        sum += p8;
    }

    free(primes);
    return sum;
}
