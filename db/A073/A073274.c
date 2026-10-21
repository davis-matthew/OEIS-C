/*
 * A073274  a(n) = A000040(n+1) - A073273(n).
 * https://oeis.org/A073274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = prime(n+1) - ⌊√(prime(n)·prime(n+2))⌋ The function uses a
 *            cached list of primes, building it up to the required index on
 *            the first call
 * Range:     i8 n <= 3, i16 n <= 30, i32 n <= 5212, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* handle the sequence offset (n>=1) – values for n<=0 are arbitrary */
    if (n < 1) return (seq_t)0;

    /* static cache of primes */
    static uint64_t *primes = NULL;
    static size_t prime_count = 0;
    static idx_t prime_limit = 0;

    /* we need primes up to index n+2 (0‑based indices are p[n‑1], p[n], p[n+1]) */
    idx_t required_idx = n + 2;
    if (required_idx > prime_count) {
        idx_t new_limit = _prime_limit_for_idx(required_idx);
        if (primes != NULL) free(primes);
        build_primes_upto(new_limit, &primes, &prime_count);
        prime_limit = new_limit;
    }

    /* retrieve primes */
    uint64_t p_n      = primes[n - 1];
    uint64_t p_n_plus1 = primes[n];
    uint64_t p_n_plus2 = primes[n + 1];

    /* compute floor(sqrt(p_n * p_n_plus2)) with arbitrary‑precision support */
    seq_t product = ((seq_t)p_n) * ((seq_t)p_n_plus2);
    seq_t root    = floor_sqrt(product);

    /* result: prime(n+1) - root */
    return ((seq_t)p_n_plus1) - root;
}
