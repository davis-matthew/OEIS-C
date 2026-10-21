/*
 * A062754  a(n) = gcd(n, sigma(n+1)).
 * https://oeis.org/A062754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, sigma(n+1))
 * Range:     u8 n <= 895, u16 n <= 9765, u32 n <= 9765, u64 n <= 9765,
 *            u128 n <= 9765, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset starts at 1, but guard against 0 */
    uint64_t m = (uint64_t)n + 1ULL;     /* compute n+1 */

    /* factorize m */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    /* compute sigma(m) modulo n */
    uint64_t mod = (uint64_t)n;
    uint64_t sig_mod = mod ? 1 % mod : 1; /* mod 1 gives 0, which is fine */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t exp = exps[i];
        uint64_t power = 1;
        uint64_t term  = 0;
        for (uint64_t j = 0; j <= exp; ++j) {
            term += power;
            if (j < exp) power *= p;
        }
        if (mod) {
            term %= mod;
            sig_mod = (sig_mod * term) % mod;
        }
    }

    /* gcd(n, sigma(m)) using Euclid on uint64_t values */
    uint64_t a_val = (uint64_t)n;
    uint64_t b_val = sig_mod;
    while (b_val != 0) {
        uint64_t t = a_val % b_val;
        a_val = b_val;
        b_val = t;
    }

    return (seq_t)a_val;  /* final value fits in seq_t when it does */
}
