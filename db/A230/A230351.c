/*
 * A230351  Number of ordered ways to write n = p + q (q > 0) with p, 2*p^2 -
 *          1 and 2*q^2 - 1 all prime.
 * https://oeis.org/A230351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (p,q) such that n = p + q, q > 0,
 *            and both p, 2*p*p - 1 and 2*q*q - 1 are prime
 * Range:     u8 n <= 13, u16 n <= 1042, u32 n <= 1042, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/build_primes_upto.h"

/* Static storage for primes up to the maximum n seen so far. */
static uint64_t *prime_arr = NULL;
static size_t    prime_cnt = 0;
static idx_t     prime_limit = 0;

/* Ensure that prime_arr contains all primes ≤ limit. */
static void ensure_primes(idx_t limit) {
    if (prime_limit >= limit) return;
    uint64_t *new_arr = NULL;
    size_t    new_cnt = 0;
    build_primes_upto(limit, &new_arr, &new_cnt);
    if (prime_arr) free(prime_arr);
    prime_arr   = new_arr;
    prime_cnt   = new_cnt;
    prime_limit = limit;
}

/* OEIS A230351 */
seq_t a(idx_t n) {
    if (n <= 1) return 0;                /* no p < n when n <= 1 */
    idx_t up = n - 1;
    ensure_primes(up);

    seq_t count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_arr[i];
        if (p >= n) break;
        uint64_t q = n - p;
        seq_t val_p = ((seq_t)2 * (seq_t)p * (seq_t)p) - (seq_t)1;
        if (!is_prime_big(val_p)) continue;
        seq_t val_q = ((seq_t)2 * (seq_t)q * (seq_t)q) - (seq_t)1;
        if (!is_prime_big(val_q)) continue;
        ++count;
    }
    return count;
}
