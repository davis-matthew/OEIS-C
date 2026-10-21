/*
 * A098104  Numbers of the form 7^p - p^7 where p is prime.
 * https://oeis.org/A098104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 7^p_n – p_n^7 where p_n is the n‑th prime (n ≥ 1)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 8, i128 n <= 12,
 *            big any n
 * Helpers:   _prime_limit_for_idx, big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/big_pow.h"

/* Compute the nth term of A098104 */
seq_t a(idx_t n)
{
    /* a(0) is undefined – return 0 for safety */
    if (n == 0)
        return 0;

    /* Estimate an upper bound for the n‑th prime and generate primes up to that bound */
    idx_t   limit  = _prime_limit_for_idx(n);
    uint64_t *primes = NULL;
    size_t   cnt     = 0;

    build_primes_upto(limit, &primes, &cnt);

    /* If the estimate was too low, grow the limit and retry */
    while ((size_t)(n - 1) >= cnt) {
        limit <<= 1;                    /* double the bound */
        free(primes);
        primes = NULL;
        cnt    = 0;
        build_primes_upto(limit, &primes, &cnt);
    }

    /* nth prime (0‑based index) */
    seq_t p = (seq_t)primes[n - 1];

    /* Compute 7^p and p^7 */
    seq_t seven_pow_p = big_pow((seq_t)7, p);
    seq_t p_pow_seven = big_pow(p, (seq_t)7);

    /* Result is their difference */
    seq_t result = seven_pow_p - p_pow_seven;

    free(primes);
    return result;
}
