/*
 * A082572  a(n) is the least number m such that the arithmetic mean of the
 *          distinct primes dividing m equals n.
 * https://oeis.org/A082572
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 48, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: minimal product of distinct primes whose arithmetic mean equals n.
   This implementation searches for solutions with 1, 2, or 3 primes and
   returns the smallest product m.  It uses a sieve up to 3*n for prime
   testing. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static bool *prime_sieve = NULL;
static size_t sieve_limit = 0;
static int *prime_list = NULL;
static size_t prime_cnt = 0;

/* Build / extend the prime sieve up to at least 'limit'.  The list of
   primes is also regenerated. */
static void ensure_sieve(size_t limit)
{
    if (limit <= sieve_limit)
        return;

    size_t new_limit = limit;
    bool *new_sieve = (bool *)malloc((new_limit + 1) * sizeof(bool));
    if (!new_sieve)
        return;   /* allocation failure – unlikely in test environment */

    for (size_t i = 0; i <= new_limit; ++i)
        new_sieve[i] = true;
    new_sieve[0] = new_sieve[1] = false;

    for (size_t i = 2; i * i <= new_limit; ++i) {
        if (new_sieve[i]) {
            for (size_t j = i * i; j <= new_limit; j += i)
                new_sieve[j] = false;
        }
    }

    free(prime_sieve);
    prime_sieve = new_sieve;
    sieve_limit = new_limit;

    /* rebuild prime list */
    size_t cnt = 0;
    for (size_t i = 2; i <= new_limit; ++i)
        if (new_sieve[i])
            ++cnt;

    int *new_list = (int *)malloc(cnt * sizeof(int));
    size_t idx = 0;
    for (size_t i = 2; i <= new_limit; ++i)
        if (new_sieve[i])
            new_list[idx++] = (int)i;

    free(prime_list);
    prime_list = new_list;
    prime_cnt = cnt;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;   /* undefined for n<2 */

    size_t max_needed = 3 * (size_t)n;
    ensure_sieve(max_needed);

    /* k = 1 : n must itself be prime */
    if (n <= sieve_limit && prime_sieve[n])
        return (seq_t)n;

    seq_t best = 0;
    bool found = false;

    /* k = 2 : p + q = 2n, distinct primes */
    if (2 * (size_t)n <= sieve_limit) {
        for (size_t i = 0; i < prime_cnt; ++i) {
            size_t p = (size_t)prime_list[i];
            if (p >= n)
                break;
            size_t q = 2 * (size_t)n - p;
            if (q <= p)
                continue;
            if (q <= sieve_limit && prime_sieve[q]) {
                seq_t prod = (seq_t)p * q;
                if (!found || prod < best) {
                    best = prod;
                    found = true;
                }
                /* first valid pair gives minimal product for k=2,
                   but we continue to compare with k=3 candidates */
            }
        }
    }

    /* k = 3 : p + q + r = 3n, distinct primes */
    size_t sum3 = 3 * (size_t)n;
    for (size_t i = 0; i < prime_cnt; ++i) {
        size_t p = (size_t)prime_list[i];
        if (p > sum3 - 5)   /* need at least 2 + 3 left for q,r */
            break;
        for (size_t j = i + 1; j < prime_cnt; ++j) {
            size_t q = (size_t)prime_list[j];
            if (p + q > sum3 - 2)   /* at least 2 left for r */
                break;
            size_t r = sum3 - p - q;
            if (r <= q)
                continue;
            if (r <= sieve_limit && prime_sieve[r]) {
                seq_t prod = (seq_t)p * q;
                prod *= r;
                if (!found || prod < best) {
                    best = prod;
                    found = true;
                }
            }
        }
    }

    return found ? best : 0;
}
