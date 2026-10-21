/*
 * A076981  Smallest k such that n*(n+1)*(n+2)*...*(n+k) is divisible by the
 *          product of primes up to n.
 * https://oeis.org/A076981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = max_{p ≤ n} ((p - n mod p) mod p)
 *          that is, the smallest k so that the range [n, n+k]
 *          contains a multiple of every prime p ≤ n.
 */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static idx_t sieve_limit = 1;
static bool *is_prime = NULL;
static idx_t *prime_list = NULL;
static size_t prime_cnt = 0;

/* Extend the sieve and prime list up to 'limit' (inclusive). */
static void extend_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;

    bool *new_is_prime = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    if (!new_is_prime) return; /* allocation failure; should not happen */

    for (idx_t i = 2; i <= limit; ++i) new_is_prime[i] = true;
    new_is_prime[0] = new_is_prime[1] = false;

    for (idx_t i = 2; (idx_t)i * i <= limit; ++i) {
        if (new_is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                new_is_prime[j] = false;
            }
        }
    }

    size_t new_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (new_is_prime[i]) ++new_cnt;
    }

    idx_t *new_list = (idx_t *)malloc(sizeof(idx_t) * new_cnt);
    if (!new_list) {
        free(new_is_prime);
        return;
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (new_is_prime[i]) {
            new_list[idx++] = i;
        }
    }

    free(is_prime);
    free(prime_list);
    is_prime = new_is_prime;
    prime_list = new_list;
    prime_cnt = new_cnt;
    sieve_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1 */
    extend_sieve(n);
    seq_t best = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p = (seq_t)prime_list[i];
        seq_t r = (seq_t)n % p;
        seq_t t = r == 0 ? 0 : p - r;
        if (t > best) best = t;
    }
    return best;
}
