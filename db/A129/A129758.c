/*
 * A129758  Smallest prime p such that there are primes q and r with the
 *          property that p, q and r form an arithmetic progression and their
 *          sum is the same as three times the n-th prime number.
 * https://oeis.org/A129758
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the largest prime p < prime(n) such that 2*prime(n) – p
 *            is also prime (prime(n) is the n‑th prime, 1‑based, and n ≥ 3)
 * Range:     u8 n <= 47, u16 n <= 6544, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sieve_primes.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;           /* undefined for n < 3 */

    /* Static storage for a growing table of primes up to some limit. */
    static idx_t *prime_tbl = NULL;
    static size_t prime_cnt = 0;      /* number of primes stored */
    static idx_t prime_limit = 0;     /* maximum number for which primes are known */

    /* Estimate an upper bound for the n‑th prime.
       Use a generous factor to avoid resieving for n up to ~10⁴. */
    idx_t limit = n * 20ULL + 100ULL;
    if (prime_cnt < (size_t)n || limit > prime_limit) {
        /* Recompute the sieve up to the larger limit. */
        free(prime_tbl);
        sieve_primes(limit, &prime_tbl, &prime_cnt);
        prime_limit = limit;
    }

    /* Retrieve the n‑th prime.  The array is 0‑indexed. */
    seq_t pn = (seq_t)prime_tbl[(size_t)(n - 1)];

    /* Search downward for the greatest p < pn with 2*pn – p prime. */
    seq_t answer = 0;
    for (size_t i = n - 2; ; --i) {
        seq_t p = (seq_t)prime_tbl[i];
        seq_t r = 2 * pn - p;
        if (is_prime((idx_t)r)) {
            answer = p;
            break;
        }
        if (i == 0)
            break;          /* should never happen for n ≥ 3 */
    }

    return answer;
}
