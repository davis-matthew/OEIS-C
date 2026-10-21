/*
 * A058698  a(n) = p(P(n)), P = primes (A000040), p = partition numbers
 *          (A000041).
 * https://oeis.org/A058698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = partition(prime(n))
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 31, u64 n <= 80, u128 n <= 231,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = p(P(n)), where P(n) is the n-th prime and p(k) is the
 * partition number of k.  The implementation uses a simple sieve to generate
 * all primes up to a fixed upper bound and an incremental dynamic
 * programming approach to compute partition numbers using Euler's
 * pentagonal‑number recurrence.  Results are memoised for any order of
 * calls. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---- Prime generation ---- */
#define PRIME_SIEVE_LIMIT 200000 /* enough for n up to about 10000 */

static uint32_t *prime_list = NULL;
static size_t   prime_count = 0;
static bool     primes_ready = false;

static void init_primes(void)
{
    if (primes_ready)
        return;
    bool *is_prime = (bool *)malloc((PRIME_SIEVE_LIMIT + 1) * sizeof(bool));
    if (!is_prime)
        return;
    for (size_t i = 0; i <= PRIME_SIEVE_LIMIT; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t p = 2; p * p <= PRIME_SIEVE_LIMIT; ++p)
        if (is_prime[p])
            for (size_t j = p * p; j <= PRIME_SIEVE_LIMIT; j += p)
                is_prime[j] = false;

    /* Count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (is_prime[i])
            ++cnt;

    prime_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!prime_list) {
        free(is_prime);
        return;
    }

    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (is_prime[i])
            prime_list[idx++] = (uint32_t)i;

    prime_count = cnt;
    primes_ready = true;
    free(is_prime);
}

/* ---- Partition computation ---- */
static seq_t *p_value = NULL; /* array of p(k) for k = 0 .. curr_max_k */
static idx_t   curr_max_k = 0; /* largest k for which p(k) is stored */

static void compute_partitions_upto(idx_t max_k)
{
    if (max_k <= curr_max_k)
        return;

    /* Allocate / expand the array */
    seq_t *new_p = (seq_t *)realloc(p_value, (max_k + 1) * sizeof(seq_t));
    if (!new_p)
        return; // allocation failed, leave function
    p_value = new_p;

    /* Ensure p(0) = 1 */
    if (curr_max_k == 0 && p_value[0] == 0)
        p_value[0] = 1;

    for (idx_t n = curr_max_k + 1; n <= max_k; ++n) {
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;
        for (idx_t k = 1;; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > n)
                break;
            seq_t term = p_value[n - pent1];
            if (k & 1) /* k odd -> positive */
                pos_sum += term;
            else
                neg_sum += term;

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > n)
                continue;
            term = p_value[n - pent2];
            if (k & 1)
                pos_sum += term;
            else
                neg_sum += term;
        }

        /* p(n) = pos_sum - neg_sum  (pos_sum >= neg_sum) */
        seq_t val = pos_sum;
        val -= neg_sum;
        p_value[n] = val;
    }
    curr_max_k = max_k;
}

/* ---- Public sequence function ---- */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* value for n < 1 is undefined, return 0 */

    init_primes();
    if (n > prime_count)
        return 0; /* out of precomputed range */

    uint32_t prime = prime_list[n - 1];
    compute_partitions_upto(prime);
    return p_value[prime];
}
