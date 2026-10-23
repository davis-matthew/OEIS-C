/*
 * A122103  Sum of the fifth powers of the first n primes.
 * https://oeis.org/A122103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1..n} prime(k)^5
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 19, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

/* Sum of the fifth powers of the first n primes. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* behaviour below the offset is irrelevant */

    uint32_t *primes = NULL;
    size_t cnt = 0;
    sieve_first_n_primes(n, &primes, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t p5 = p * p * p * p * p;   /* p^5 */
        sum += p5;
    }

    free(primes);
    return sum;
}
