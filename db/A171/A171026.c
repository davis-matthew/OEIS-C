/*
 * A171026  In the sequence of prime numbers, replace all the '5' digits with
 *          '3' and vice versa.
 * https://oeis.org/A171026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = swap35( prime(n) ), i.e. the n‑th prime with * all
 *            decimal digits 5 and 3 exchanged
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes, swap35
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes.h"
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/swap35.h"

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined for n<1, value irrelevant */
        return (seq_t)0;

    /* Obtain an upper bound for the n‑th prime. */
    idx_t limit = _prime_limit_for_idx(n);

    /* Generate all primes up to the bound. */
    idx_t *primes;
    size_t primes_cnt;
    build_primes(limit, &primes, &primes_cnt);

    /* The limit is chosen so that primes_cnt >= n. */
    seq_t result = swap35((seq_t)primes[n - 1]);

    /* Clean up. */
    free(primes);

    return result;
}
