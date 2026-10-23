/*
 * A046864  Smallest number whose digits sum to n-th prime.
 * https://oeis.org/A046864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = smallest integer whose digit sum equals the n‑th prime
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes.h"

/* Memoised list of primes */
static idx_t *primes = NULL;
static size_t primes_cnt = 0;
static idx_t primes_lim = 0;

/* Return the n-th prime (1‑based).  Memoises the primes up to a
   suitable limit, rebuilding if a larger n is requested. */
static idx_t get_nth_prime(idx_t n)
{
    if (n == 0) return 0;        /* safety: offset is 1 */
    idx_t limit = _prime_limit_for_idx(n);
    if ((idx_t)limit > primes_lim) {
        if (primes != NULL)
            free(primes);
        primes = NULL;
        build_primes(limit, &primes, &primes_cnt);
        primes_lim = limit;
    }
    /* The limit is chosen so that primes_cnt >= n. */
    return primes[n - 1];
}

/* Compute a(n).  The result fits into seq_t if it fits, otherwise
   seq_t is expected to be an arbitrary‑precision type in the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for offset < 1 */
    idx_t prime = get_nth_prime(n);

    seq_t q = prime / 9;              /* number of trailing 9's */
    int r = (int)(prime % 9);          /* leading digit, if any */

    seq_t res = 0;
    if (r == 0) {
        for (size_t i = 0; i < (size_t)q; ++i)
            res = res * 10 + 9;
    } else {
        res = r;
        for (size_t i = 0; i < (size_t)q; ++i)
            res = res * 10 + 9;
    }
    return res;
}
