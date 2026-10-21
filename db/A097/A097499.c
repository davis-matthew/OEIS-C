/*
 * A097499  Numbers of the form p^q + q^p where p and q are consecutive
 *          primes.
 * https://oeis.org/A097499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p^q + q^p, where p and q are consecutive primes
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses the helper functions build_primes_upto and big_pow. */

#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/big_pow.h"

/* Compute the n-th term of OEIS A097499.
   n = 1 corresponds to the first term (p=2,q=3). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    /* Static storage for the prime list and its size. */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_limit = 200000;   /* initial upper bound for primes */

    /* Build or extend the prime list on first call or if needed. */
    if (primes == NULL) {
        build_primes_upto(prime_limit, &primes, &prime_cnt);
    } else if (n + 1 >= prime_cnt) {   /* need more primes for this n */
        /* increase limit by a factor (or add a fixed amount) */
        while (n + 1 >= prime_cnt) {
            prime_limit *= 2;
        }
        /* Build a larger prime table; old primes are left leaked (kept for simplicity). */
        build_primes_upto(prime_limit, &primes, &prime_cnt);
    }

    /* Retrieve the nth and (n+1)th primes (0‑based indexing). */
    uint64_t p = primes[n - 1];
    uint64_t q = primes[n];

    /* Compute p^q and q^p using arbitrary precision arithmetic. */
    seq_t pow1 = big_pow((seq_t)p, (seq_t)q);
    seq_t pow2 = big_pow((seq_t)q, (seq_t)p);

    return pow1 + pow2;
}
