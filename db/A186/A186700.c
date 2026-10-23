/*
 * A186700  Next palindrome after n-th prime.
 * https://oeis.org/A186700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 6538, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_palindrome, prime_limit_for_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Next palindrome after the n-th prime.
   a(n) = the smallest palindrome > p_n, where p_n is the n-th prime. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_palindrome.h"
#include "../utils/build_primes_upto.h"
#include "../utils/prime_limit_for_idx.h"

static uint64_t *primes = NULL;      /* primes up to primes_limit */
static size_t   prime_cnt = 0;       /* number of primes stored */
static idx_t    primes_limit = 0;    /* maximum value of primes generated */

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* offset is 1, undefined below 1 */

    /* Ensure we have at least n primes */
    if (prime_cnt < (size_t)n) {
        idx_t limit = prime_limit_for_idx(n);   /* upper bound for p_n */
        if (limit > primes_limit || primes == NULL) {
            uint64_t *new_primes = NULL;
            size_t   new_cnt = 0;
            build_primes_upto(limit, &new_primes, &new_cnt);
            if (primes) free(primes);
            primes = new_primes;
            prime_cnt = new_cnt;
            primes_limit = limit;
        }
        /* else existing array already includes enough primes */
    }

    /* Retrieve the n-th prime (array is zero‑indexed) */
    uint64_t p_big = primes[(size_t)(n - 1)];
    seq_t    p = (seq_t)p_big;

    /* Find the next palindrome strictly greater than p */
    seq_t cand = p + (seq_t)1;
    while (!is_palindrome(cand, 10))
        cand += (seq_t)1;

    return cand;
}
