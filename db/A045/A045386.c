/*
 * A045386  Primes congruent to {1, 2, 4} mod 7.
 * https://oeis.org/A045386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that p ≡ 1, 2 or 4 (mod 7)
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes by sieving primes up to an upper bound that guarantees at least n qualifying primes. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n == 0)                    /* undefined for offset 0 – return 0 */
        return (seq_t)0;

    /* Rough upper bound: 30 * n + 100 guarantees > 2*n primes for n ≥ 1 */
    idx_t limit = n * 30 + 100;

    uint64_t *primes = NULL;      /* array ofuint64_t primes up to limit */
    size_t   prime_cnt = 0;       /* number of primes returned */

    /* Build all primes up to limit.  The function allocates memory that must be freed. */
    build_primes_upto(limit, &primes, &prime_cnt);

    seq_t  result = 0;
    size_t qcnt   = 0;            /* count of qualifying primes seen so far */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        uint8_t r = p % 7;
        if (r == 1 || r == 2 || r == 4) {
            ++qcnt;
            if (qcnt == (size_t)n) {   /* found the n-th qualifying prime */
                result = (seq_t)p;
                break;
            }
        }
    }

    free(primes);            /* release memory allocated by build_primes_upto */

    return result;
}
