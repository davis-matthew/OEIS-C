/*
 * A038394  Concatenate first n primes in reverse order.
 * https://oeis.org/A038394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Concatenate first n primes in reverse order
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   digit_count, pow10, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"
#include "../utils/digit_count.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    uint32_t *primes = NULL;
    size_t cnt = 0;
    sieve_first_n_primes(n, &primes, &cnt);
    if (cnt < (size_t)n) return (seq_t)0;   /* defensive, should not happen */

    seq_t result = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i) {
        uint32_t p = primes[cnt - 1 - i];
        uint64_t d = digit_count((uint64_t)p);
        seq_t pow = pow10((idx_t)d);
        result = result * pow + (seq_t)p;
    }

    free(primes);
    return result;
}
