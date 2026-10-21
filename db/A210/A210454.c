/*
 * A210454  Cipolla pseudoprimes to base 2: (4^p-1)/3 for any prime p greater
 *          than 3.
 * https://oeis.org/A210454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4^(p_{n+2}) - 1) / 3 where p_k is the k‑th prime (>3)
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The array of all primes needed by this sequence.  It is built
 * lazily and expanded on demand.  The first call to a() creates
 * a prime list up to the 200000th number, which contains all
 * primes up to 179842, enough for n up to at least 10000. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/build_primes_upto.h"

/* static state belonging to this function */
static uint64_t *prime_list   = NULL; /* array of primes, prime_list[0] = 2 */
static size_t   prime_count   = 0;    /* number of primes stored            */
static idx_t    prime_limit   = 0;    /* current limit used in sieve       */

/* ensure that prime_list contains at least (idx_needed+1) primes
 * (idx_needed is a zero‑based index).  If not, grow the sieve. */
static void
ensure_primes_upto(idx_t idx_needed)
{
    /* initial sieve covering the 10000‑th prime */
    if (prime_limit == 0) {
        prime_limit = 200000;
        build_primes_upto(prime_limit, &prime_list, &prime_count);
    }
    /* grow as required */
    while ((idx_t)prime_count <= idx_needed) {
        prime_limit *= 2;            /* expand range */
        free(prime_list);
        build_primes_upto(prime_limit, &prime_list, &prime_count);
    }
}

/* return the n‑th term of OEIS A210454 (n ≥ 1) */
seq_t
a(idx_t n)
{
    /* a(1) uses the third prime (5), which is prime_list[2] */
    idx_t prime_idx = n + 1; /* zero‑based index of requested prime */

    ensure_primes_upto(prime_idx);

    uint64_t p = prime_list[prime_idx];           /* prime > 3 */
    seq_t    pow = big_pow((seq_t)4, (seq_t)p);    /* 4^p   */
    seq_t    result = (pow - (seq_t)1) / (seq_t)3; /* (4^p-1)/3 */
    return result;
}
