/*
 * A081402  a(n) = A008475(n!).
 * https://oeis.org/A081402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p <= n} p^{e_p(n!)} where e_p(n!) = sum_{k>=1}
 *            floor(n / p^k) (Legendre's formula)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    if (n <= 1)
        return 0;  /* 1! has no prime factor */

    /* Sieve of Eratosthenes to find primes up to n */
    char *is_prime = (char *)malloc((size_t)(n + 1));
    if (!is_prime)
        return 0;  /* allocation failure fallback */

    for (idx_t i = 0; i <= n; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (idx_t i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                is_prime[j] = 0;
        }
    }

    seq_t result = 0;
    for (idx_t p = 2; p <= n; ++p) {
        if (!is_prime[p])
            continue;

        /* compute exponent of prime p in n! */
        idx_t exp = 0;
        idx_t power = p;
        while (power <= n) {
            exp += n / power;
            if (power > n / p)  /* avoid overflow */
                break;
            power *= p;
        }

        /* add p^exp to result */
        seq_t term = pow_uint(p, exp);
        result += term;
    }

    free(is_prime);
    return result;
}
