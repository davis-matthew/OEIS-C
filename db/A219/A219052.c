/*
 * A219052  Number of ways to write n = p + q(3 - (-1)^n)/2 with q <= n/2 and
 *          p, q, p^2 + q^2 - 1 all prime.
 * https://oeis.org/A219052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = number of primes q ≤ n/2 such that p = n - q*(3-(-1)^n)/2
 *            is prime and p^2 + q^2 - 1 is prime
 * Range:     u8 n <= 19, u16 n <= 1101, u32 n <= 1101, u64 n <= 9553,
 *            u128 n <= 9553, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/build_primes_upto.h"

/* Store the prime list and its current maximum */
static uint64_t *prime_list = NULL;
static size_t   prime_cnt   = 0;
static uint64_t  prime_max   = 0;

/* Ensure we have primes up to at least 'limit'. */
static void ensure_primes_up_to(uint64_t limit)
{
    if (prime_list != NULL && limit <= prime_max) return;
    uint64_t *new_list = NULL;
    size_t   new_cnt   = 0;
    build_primes_upto(limit, &new_list, &new_cnt);
    free(prime_list);
    prime_list = new_list;
    prime_cnt  = new_cnt;
    prime_max  = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* factor = 1 if n even, 2 if n odd */
    uint64_t factor = (n % 2 == 0) ? 1 : 2;
    uint64_t limit_q = n / 2;                  /* q ≤ n/2 */

    ensure_primes_up_to(limit_q);

    seq_t count = 0;
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= limit_q; ++i) {
        uint64_t q = prime_list[i];
        uint64_t p = n - factor * q;
        if (p == 0) continue;                  /* p must be positive */
        if (!is_prime_big((seq_t)p)) continue; /* p must be prime */

        seq_t val = (seq_t)p * (seq_t)p
                  + (seq_t)q * (seq_t)q
                  - (seq_t)1;
        if (is_prime_big(val)) count++;
    }
    return count;
}
