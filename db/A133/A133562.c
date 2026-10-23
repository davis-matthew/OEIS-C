/*
 * A133562  Numbers which are the sum of the squares of seven consecutive
 *          primes.
 * https://oeis.org/A133562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=0..6} p_{n+i}^2 where p_k is the k-th prime (p_1 =
 *            2)
 * Range:     u8 none, u16 n <= 22, u32 n <= 2737, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static idx_t upper_bound_prime(idx_t n)
{
    if (n < 6) return 15;
    double dn   = (double)n;
    double ub   = dn * (log(dn) + log(log(dn)));
    return (idx_t)(ub + 3.0);                    /* safety margin */
}

static void sieve_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    bool *is_comp = (bool*)calloc((size_t)limit + 1, sizeof(bool));
    if (!is_comp) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    size_t est = (size_t)(limit / (log(limit < 2 ? 2 : limit)) + 10);
    uint64_t *pr = (uint64_t*)malloc(est * sizeof(uint64_t));
    if (!pr) {
        free(is_comp);
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[(size_t)i]) {
            if (count == est) {
                est *= 2;
                uint64_t *tmp = (uint64_t*)realloc(pr, est * sizeof(uint64_t));
                if (!tmp) {
                    free(pr);
                    free(is_comp);
                    *primes_out = NULL;
                    *cnt_out = 0;
                    return;
                }
                pr = tmp;
            }
            pr[count++] = (uint64_t)i;

            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[(size_t)j] = true;
            }
        }
    }

    uint64_t *tmp = (uint64_t*)realloc(pr, count * sizeof(uint64_t));
    if (tmp) pr = tmp;

    *primes_out = pr;
    *cnt_out = count;
    free(is_comp);
}

static uint64_t *cached_primes = NULL;
static size_t cached_prime_count = 0;
static idx_t cached_sieve_limit = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n < 1 */

    idx_t required_primes = n + 6;     /* need p_n ... p_{n+6} */

    if (required_primes > cached_prime_count) {
        idx_t new_limit = upper_bound_prime(required_primes);
        uint64_t *new_primes = NULL;
        size_t new_count = 0;
        sieve_upto(new_limit, &new_primes, &new_count);

        free(cached_primes);
        cached_primes = new_primes;
        cached_prime_count = new_count;
        cached_sieve_limit = new_limit;
    }

    seq_t sum = 0;
    for (idx_t i = 0; i < 7; ++i) {
        seq_t p = (seq_t)cached_primes[(size_t)(n - 1 + i)];
        sum += p * p;
    }
    return sum;
}
