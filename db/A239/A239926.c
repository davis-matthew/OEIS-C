/*
 * A239926  3^(p-1)-2^(p+1) for primes p > 3.
 * https://oeis.org/A239926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 3^(p-1) - 2^(p+1) where p is the (n+1)th prime (p > 3)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   build_primes_upto, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/* Helper function prototypes provided by the build environment */
#include "../utils/pow_int.h"          /* seq_t pow_int(seq_t base, idx_t exp); */
#include "../utils/build_primes_upto.h" /* void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out); */

seq_t a(idx_t n)
{
    /* Static storage for the prime list and its count */
    static uint64_t *primes = NULL;    /* primes[0] = 2, primes[1] = 3, primes[2] = 5, ... */
    static size_t prime_cnt = 0;       /* number of primes generated */
    static bool initialized = false;   /* whether the prime array has been built */

    /* Build a sieve large enough to cover n up to 10,000 (p ≤ 104729) */
    if (!initialized) {
        /* 200000 gives ~18,000 primes, enough for n ≤ 10000 */
        const idx_t sieve_limit = 200000;
        build_primes_upto(sieve_limit, &primes, &prime_cnt);
        initialized = true;
    }

    /* Ensure we have at least (n+2) primes (since primes[2] is the first prime > 3) */
    if (prime_cnt <= (size_t)(n + 1)) {
        /* If more primes are needed, extend the sieve */
        /* Allocate a larger limit; this should rarely happen for n ≤ 10000 */
        idx_t new_limit = (idx_t)(prime_cnt * 2);
        uint64_t *new_primes = NULL;
        size_t new_cnt = 0;
        build_primes_upto(new_limit, &new_primes, &new_cnt);
        /* Replace the old array */
        free(primes);
        primes = new_primes;
        prime_cnt = new_cnt;
    }

    /* Retrieve the (n+1)th prime > 3 */
    uint64_t p = primes[n + 1];             /* primes[0]=2, primes[1]=3, primes[2]=5 */

    /* Compute 3^(p-1) – 2^(p+1) using the provided power routine */
    seq_t term   = pow_int((seq_t)3, (idx_t)(p - 1));
    seq_t sub    = pow_int((seq_t)2, (idx_t)(p + 1));
    term -= sub;

    return term;
}
