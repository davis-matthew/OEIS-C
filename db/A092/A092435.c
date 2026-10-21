/*
 * A092435  Prime factorials divided by their corresponding primorials.
 * https://oeis.org/A092435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pₙ! / pₙ# (prime factorial divided by primorial)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   build_primes, power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include "../utils/build_primes.h"
#include "../utils/power.h"

/* Estimate an upper bound for the n‑th prime (prime number theorem). */
static idx_t estimate_limit(idx_t n) {
    if (n < 6) return 15;                     /* first few primes */
    double dn = (double) n;
    double est = dn * (log(dn) + log(log(dn)));
    return (idx_t)(est + 3.0);                /* small safety margin */
}

/* Legendre exponent of prime p in n! */
static idx_t legendre_exponent(idx_t n, idx_t p) {
    idx_t e = 0;
    while (n >= p) {
        n /= p;
        e += n;
    }
    return e;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* a(0) is undefined, return 0 */

    /* Build a list of primes up to at least the n‑th prime. */
    idx_t limit = estimate_limit(n);
    idx_t *prime_list = NULL;
    size_t prime_cnt = 0;
    build_primes(limit, &prime_list, &prime_cnt);

    while (prime_cnt < n) {
        free(prime_list);
        limit *= 2;
        build_primes(limit, &prime_list, &prime_cnt);
    }

    idx_t p_n = prime_list[n - 1];
    seq_t result = (seq_t)1;

    for (idx_t i = 0; i < n; ++i) {
        idx_t q = prime_list[i];
        idx_t e = legendre_exponent(p_n, q);
        if (e <= 1) continue;                 /* exponent - 1 <= 0, skip */
        seq_t pow_val = power((seq_t)q, e - 1);
        result *= pow_val;
    }

    free(prime_list);
    return result;
}
