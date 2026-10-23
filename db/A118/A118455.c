/*
 * A118455  a(1)=1, a(n) = Product_{k=2..n} P(k), where P(k) is the largest
 *          prime <= k.
 * https://oeis.org/A118455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   build_primes, pow_int, seq_mul, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A118455: a(1) = 1, a(n) = Π_{k=2..n} largest_prime_leq(k) */

#include <stdlib.h>
#include "../utils/seq_one.h"
#include "../utils/seq_mul.h"
#include "../utils/pow_int.h"
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    if (n < 2) {
        return seq_one();
    }

    /* Obtain all primes up to n (inclusive). */
    idx_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes(n, &primes, &prime_cnt);

    seq_t result = seq_one();

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p       = primes[i];
        idx_t next_p  = (i + 1 < prime_cnt) ? primes[i + 1] : n + 1;
        idx_t exp     = (next_p <= n) ? next_p - p : n + 1 - p;

        seq_t term = pow_int((seq_t)p, exp);
        result = seq_mul(result, term);
    }

    free(primes);
    return result;
}
