/*
 * A203522  a(n) = v(n+1)/v(n), where v = A203521.
 * https://oeis.org/A203522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} (prime(i)+prime(n+1))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Static storage for the first primes needed by the sequence. */
static uint64_t *prime_list   = NULL;
static size_t   prime_cap    = 0;
static size_t   prime_cnt    = 0;

/* Ensure that prime_list contains at least (n+1) primes (indices 0..n). */
static void
ensure_primes(idx_t n)
{
    if (prime_cnt > n)
        return;                               /* already enough */

    /* Expand storage if necessary */
    if (prime_cap < (size_t)(n + 1)) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 8;
        if (new_cap < (size_t)(n + 1))
            new_cap = n + 1;
        uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                            new_cap * sizeof(uint64_t));
        if (!tmp)
            exit(1);                           /* allocation failed */
        prime_list = tmp;
        prime_cap  = new_cap;
    }

    /* Generate missing primes by trial division, starting from last prime+1 */
    uint64_t candidate =
        (prime_cnt == 0) ? 2 : prime_list[prime_cnt - 1] + 1;
    while (prime_cnt <= (size_t)n) {
        bool isprime = true;
        for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= candidate; ++i) {
            if (candidate % prime_list[i] == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            prime_list[prime_cnt++] = candidate;
            candidate = prime_list[prime_cnt - 1] + 1;
        } else {
            ++candidate;
        }
    }
}

seq_t a(idx_t n)
{
    /* n is the index in the sequence, offset 0.  Need primes 1..n+1. */
    ensure_primes(n + 1);                /* ensure primes[0..n] exist */

    seq_t result = (seq_t)1;
    uint64_t offset = prime_list[n];     /* prime(n+1) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t term = (seq_t)(prime_list[i - 1] + offset);
        result *= term;
    }
    return result;
}
