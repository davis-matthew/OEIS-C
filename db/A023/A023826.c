/*
 * A023826  Sum of exponents in prime-power factorization of C(4n,n).
 * https://oeis.org/A023826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Ω( C(4n, n) ) = Σ_{p prime} Σ_{k≥1} (⌊4n / p^k⌋ - ⌊n /
 *            p^k⌋ - ⌊3n / p^k⌋)
 * Range:     u8 n <= 756, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes.h"

static idx_t *primes_global = NULL;
static size_t  primes_cnt   = 0;
static idx_t   primes_limit = 0;

seq_t a(idx_t n)
{
    idx_t n4 = (idx_t)4 * n;          /* 4*n */
    idx_t n3 = (idx_t)3 * n;          /* 3*n */

    /* Ensure primes up to 4*n are available */
    if (n4 > primes_limit) {
        if (primes_global)
            free(primes_global);
        build_primes(n4, &primes_global, &primes_cnt);
        primes_limit = n4;
    }

    seq_t total = 0;

    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes_global[i];
        if (p > n4)
            break;

        idx_t pk = p;
        while (pk <= n4) {
            /* Compute exponent contribution for this prime power */
            seq_t inc = (seq_t)(n4 / pk) - (seq_t)(n / pk) - (seq_t)(n3 / pk);
            total += inc;

            /* Prevent overflow in next power */
            if (pk > n4 / p)
                break;
            pk *= p;
        }
    }

    return total;
}
