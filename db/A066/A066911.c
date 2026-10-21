/*
 * A066911  Sum of primes < n that do not divide n.
 * https://oeis.org/A066911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of all primes p < n such that p∤n
 * Range:     u8 n <= 43, u16 n <= 929, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Prime sieve data                                         */
static idx_t    sieve_limit = 0;          /* largest n for which data are known (exclusive) */
static uint8_t *is_prime_arr = NULL;      /* 1 = prime, 0 = composite, indexed by i */
static seq_t  *prime_psum = NULL;         /* prime_psum[i] = sum_{p<i} p   (i >= 0) */
static idx_t  *primes_list = NULL;        /* list of all primes < sieve_limit */
static size_t  primes_cnt  = 0;

/* Rebuild prime tables up to (but excluding) limit n                */
static void ensure_primes_up_to(idx_t n)
{
    if (n <= sieve_limit)               /* already computed */
        return;

    /* Allocate new prime‑flag array. */
    size_t sz = (size_t)n;
    uint8_t *new_flag = (uint8_t*)malloc(sz);
    if (!new_flag) abort();
    for (size_t i = 0; i < sz; ++i) new_flag[i] = 1;
    if (sz > 0) new_flag[0] = 0;
    if (sz > 1) new_flag[1] = 0;

    /* Classic Eratosthenes sieve. */
    for (idx_t i = 2; (idx_t)i * i < n; ++i) {
        if (new_flag[i]) {
            for (idx_t j = i * i; j < n; j += i)
                new_flag[j] = 0;
        }
    }

    /* Allocate prime prefix‑sum array. */
    seq_t *new_psum = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!new_psum) abort();
    new_psum[0] = 0;
    if (sz > 1) new_psum[1] = 0;

    /* Allocate prime list.            */
    size_t cap = (sz / 10) + 10;      /* rough estimate */
    idx_t *new_primes = (idx_t*)malloc(cap * sizeof(idx_t));
    if (!new_primes) abort();

    /* Build list and prefix sums.      */
    size_t cnt = 0;
    seq_t cum  = 0;
    for (idx_t i = 2; i < n; ++i) {
        if (new_flag[i]) {
            if (cnt >= cap) {           /* grow if needed */
                cap <<= 1;
                idx_t *tmp = (idx_t*)realloc(new_primes, cap * sizeof(idx_t));
                if (!tmp) abort();
                new_primes = tmp;
            }
            new_primes[cnt++] = i;
            cum += (seq_t)i;
        }
        new_psum[i] = cum;               /* sum of primes < i+1, but we store at i */
    }

    /* Replace old data with new data.     */
    free(is_prime_arr);
    free(prime_psum);
    free(primes_list);
    is_prime_arr   = new_flag;
    prime_psum     = new_psum;
    primes_list    = new_primes;
    primes_cnt     = cnt;
    sieve_limit    = n;
}

/* Sequence term computation                                    */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;       /* offset starts at 1, no primes below 2 */

    /* Ensure prime tables cover all numbers < n. */
    ensure_primes_up_to(n);

    /* Sum of all primes < n. */
    seq_t total = prime_psum[n-1];

    /* Subtract primes that divide n. */
    idx_t temp   = n;
    seq_t div_sum = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes_list[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            div_sum += (seq_t)p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1 && temp != n)        /* remaining prime factor > sqrt(n) */
        div_sum += (seq_t)temp;

    return total - div_sum;
}
