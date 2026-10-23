/*
 * A123113  Main diagonal of prime power sum array.
 * https://oeis.org/A123113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + \sum_{k=1}^{n} n^{p_k} , where p_k is the k-th prime
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow, seq_one, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"
#include "../utils/big_pow.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return seq_one();               /* undefined for offset 1, safe value */

    /* Lazily keep the first N primes for repeated calls. */
    static uint32_t *primes = NULL;          /* array of primes */
    static size_t   prime_count = 0;         /* how many we have */

    /* Grow the prime list if the current request needs more. */
    if ((size_t)n > prime_count) {
        uint32_t *new_primes = NULL;
        size_t   new_count  = 0;
        sieve_first_n_primes(n, &new_primes, &new_count);
        if (primes)
            free(primes);                   /* free old buffer */
        primes      = new_primes;
        prime_count = new_count;
    }

    seq_t sum  = seq_one();                 /* start with the leading 1 */
    seq_t base = (seq_t)n;                  /* base for all powers */

    for (size_t i = 0; i < (size_t)n; ++i) {
        seq_t exp  = (seq_t)primes[i];
        sum += big_pow(base, exp);
    }

    return sum;
}
