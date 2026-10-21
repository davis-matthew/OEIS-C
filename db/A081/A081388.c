/*
 * A081388  Number of unitary prime divisors of the n-th Catalan number.
 * https://oeis.org/A081388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unitary prime divisors of Catalan(n)
 * Range:     u8 n <= 1310, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, v_p_of, v_p_pow_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/v_p_of.h"
#include "../utils/v_p_pow_factorial.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1, but keep safe */

    /* Catalan(1) = 1 has no prime divisors */
    if (n == 1) return (seq_t)0;

    idx_t limit = 2 * n;                 /* maximum prime needed */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;

    build_primes_upto(limit, &primes, &prime_cnt);

    seq_t count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        seq_t e = v_p_pow_factorial(2 * n, p)
                - 2 * v_p_pow_factorial(n, p)
                - v_p_of(n + 1, p);
        if (e == 1) ++count;
    }

    free(primes);
    return count;
}
