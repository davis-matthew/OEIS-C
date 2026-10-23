/*
 * A049848  a(n) is the number of primes q less than n-th prime p for which
 *          the Euclidean algorithm for p,q has exactly 2 nonzero remainders.
 * https://oeis.org/A049848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   build_primes_upto, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of primes q < pₙ (the n‑th prime) for which the Euclidean
 * algorithm between pₙ and q has exactly two non‑zero remainders. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;       /* cached list of primes up to primes_limit */
static size_t   primes_cnt = 0;
static idx_t    primes_limit = 0;     /* largest prime used in sieve */

/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* out‑of‑range safety */

    idx_t p_idx = (idx_t)get_prime(n);     /* pₙ, 1‑based */
    uint64_t p = (uint64_t)p_idx;          /* promote for arithmetic */

    /* Extend cached prime list if necessary. */
    if (p > primes_limit) {
        if (primes) free(primes);
        build_primes_upto(p, &primes, &primes_cnt);
        primes_limit = p;
    }

    seq_t count = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t q = primes[i];
        if (q >= p) break;                 /* primes are sorted */

        uint64_t r1 = p % q;
        if (r1 == 0) continue;             /* one remainder only */

        uint64_t r2 = q % r1;
        if (r2 == 0) continue;             /* only two remainders with 0 */

        if (r1 % r2 == 0) ++count;         /* exactly two non‑zero remainders */
    }

    return count;
}
