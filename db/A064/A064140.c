/*
 * A064140  Sum of unitary divisors of central binomial coefficient C(n,
 *          floor(n/2)).
 * https://oeis.org/A064140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A034448(A001405(n)) (sum of unitary divisors of the *
 *            central binomial coefficient C(n, floor(n/2)))
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 130,
 *            big any n
 * Helpers:   build_primes, pow_int_seq, v_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes.h"
#include "../utils/pow_int_seq.h"   /* provides seq_t pow_int_seq(seq_t base, idx_t exp) */
#include "../utils/v_prime_factor.h"

/* Cache of primes up to the maximum requested n */
static idx_t *primes_cache = NULL;
static size_t primes_cnt = 0;
static idx_t primes_max = 0;

static void ensure_primes(idx_t upper) {
    if (upper <= primes_max)
        return;
    /* free old array if any */
    if (primes_cache != NULL)
        free(primes_cache);
    /* build primes up to `upper` */
    build_primes(upper, &primes_cache, &primes_cnt);
    primes_max = upper;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;   /* out of range, but just in case */

    idx_t k = n / 2;   /* floor(n/2) */

    ensure_primes(n);

    seq_t res = (seq_t)1;

    for (size_t i = 0; i < primes_cnt && primes_cache[i] <= n; ++i) {
        idx_t p = primes_cache[i];
        idx_t e = v_prime_factor(n, p)
                - v_prime_factor(k, p)
                - v_prime_factor(n - k, p);
        if (e == 0)
            continue;
        /* term = 1 + p^e */
        seq_t term = pow_int_seq((seq_t)p, e);
        term = term + (seq_t)1;
        res = res * term;
    }

    return res;
}
