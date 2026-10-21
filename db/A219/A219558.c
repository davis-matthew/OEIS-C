/*
 * A219558  Number of odd prime pairs {p,q} (p>q) such that p+(1+(n mod 2))q=n
 *          and ((p-1-(n mod 2))/q)=((q+1)/p)=1 where (-) denotes the Legendre
 *          symbol.
 * https://oeis.org/A219558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A219558:
   a(n) = number of odd prime pairs {p,q} (p > q > 2) such that
          p + (1 + (n mod 2))·q = n,
          ((p - 1 - (n mod 2)) / q) = 1   and
          ((q + 1) / p)      = 1,
   where "/" denotes the Legendre symbol. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Helper utilities supplied by the build system. */
#include "../utils/jacobi_symbol.h"
#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* Cached prime table up to some maximum limit. */
    static bool          initialized = false;
    static uint64_t     *primes      = NULL;
    static size_t        prime_cnt   = 0;
    static idx_t         limit_val   = 0;

    /* Build or extend the prime table if needed. */
    if (!initialized) {
        limit_val = 1;               /* start with a trivial table */
    }
    if (n > limit_val) {
        build_primes_upto(n, &primes, &prime_cnt);
        limit_val = n;
        initialized = true;
    }

    seq_t count = 0;   /* result to return */

    /* n mod 2 == 0 : even, relation p + q = n. */
    if ((n & 1ULL) == 0ULL) {
        for (size_t i = 1; i < prime_cnt; ++i) {      /* skip prime 2 */
            uint64_t q = primes[i];
            if (q >= n) break;                       /* need q < n          */
            uint64_t p = n - q;                      /* p = n - q           */
            if (p <= q) continue;                    /* enforce p > q       */
            if (!is_prime(p)) continue;              /* p must be prime     */
            if (jacobi_symbol(p - 1ULL, q) != 1) continue;
            if (jacobi_symbol(q + 1ULL, p) != 1) continue;
            ++count;
        }
    }
    /* n mod 2 == 1 : odd, relation p + 2q = n. */
    else {
        for (size_t i = 1; i < prime_cnt; ++i) {
            uint64_t q = primes[i];
            if (2ULL * q >= n) break;                /* need 2q < n           */
            uint64_t p = n - 2ULL * q;               /* p = n - 2q            */
            if (p <= q) continue;                    /* enforce p > q         */
            if (!is_prime(p)) continue;              /* p must be prime       */
            if (jacobi_symbol(p - 2ULL, q) != 1) continue;
            if (jacobi_symbol(q + 1ULL, p) != 1) continue;
            ++count;
        }
    }

    return (seq_t)count;
}
