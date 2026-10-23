/*
 * A122807  Least positive k such that n is equal to arithmetic mean of k
 *          consecutive primes, or 0 if no such k exists.
 * https://oeis.org/A122807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 34, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Least positive k such that n is the arithmetic mean of k consecutive primes,
    or 0 if no such k exists.  A direct search over k with binary search
    over cumulative prime sums.  Primes up to a dynamic limit are generated
    lazily when needed.  */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*----- static state ---------------------------------------------------*/
static uint64_t *prime_ptr   = NULL;    /* array of primes */
static size_t    prime_cnt   = 0;        /* number of primes generated */
static idx_t     prime_limit = 0;        /* highest integer sieved */

static seq_t *prefix_sum_ptr = NULL;    /* prefix sums: size = prime_cnt + 1 */
static size_t prefix_cnt = 0;           /* current prefix array size */

/*----- helper: binary search the prime array ----------------------------*/
static bool is_prime_in_array(uint64_t v)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_ptr[mid] < v)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < prime_cnt && prime_ptr[lo] == v);
}

/*----- helper: extend the prime table up to at least 'limit' -----------*/
static void extend_primes_to(idx_t limit)
{
    if (limit <= prime_limit) return;

    /* Simple sieve of Eratosthenes */
    size_t sz = (size_t)limit + 1;
    uint8_t *sieve = (uint8_t *)malloc(sz);
    if (!sieve) return;                /* allocation failure -> abort silently */
    memset(sieve, 1, sz);
    sieve[0] = sieve[1] = 0;
    for (size_t i = 2; i * i <= (size_t)limit; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                sieve[j] = 0;
        }
    }
    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (sieve[i]) ++cnt;
    /* allocate and fill prime array */
    uint64_t *new_prime = (uint64_t *)malloc(sizeof(uint64_t) * cnt);
    if (!new_prime) { free(sieve); return; }
    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (sieve[i]) new_prime[idx++] = (uint64_t)i;
    free(sieve);

    /* replace old data */
    free(prime_ptr);
    prime_ptr = new_prime;
    prime_cnt = cnt;
    prime_limit = limit;

    /* rebuild prefix sums */
    free(prefix_sum_ptr);
    prefix_sum_ptr = (seq_t *)malloc(sizeof(seq_t) * (prime_cnt + 1));
    if (!prefix_sum_ptr) return;
    prefix_sum_ptr[0] = (seq_t)0;
    for (size_t i = 0; i < prime_cnt; ++i)
        prefix_sum_ptr[i + 1] = prefix_sum_ptr[i] + (seq_t)prime_ptr[i];
    prefix_cnt = prime_cnt + 1;
}

/*----- main sequence function ------------------------------------------*/
seq_t a(idx_t n)
{
    /* Ensure we can test primality of n itself */
    if (n <= 1) return (seq_t)0;

    /* A pragmatic upper bound for required primes: at least n plus small buffer */
    idx_t req_limit = n > 1000 ? n + 1000 : 2000;
    if (req_limit < 2000) req_limit = 2000;
    extend_primes_to(req_limit);

    /* k = 1 : n must be prime */
    if (is_prime_in_array((uint64_t)n))
        return (seq_t)1;

    /* total sum of all primes up to limit (serves as crude upper bound) */
    seq_t total_sum = prefix_sum_ptr[prime_cnt];

    /* search k = 2 .. prime_cnt */
    for (size_t k = 2; k <= prime_cnt; ++k) {
        seq_t target = (seq_t)n * (seq_t)k;   /* n * k */
        if (target > total_sum) break;        /* cannot be reached with current primes */

        /* binary search over starting index i for this k */
        size_t lo = 0, hi = prime_cnt - k;
        while (lo <= hi) {
            size_t mid = (lo + hi) >> 1;
            seq_t sum = prefix_sum_ptr[mid + k] - prefix_sum_ptr[mid];
            if (sum == target) return (seq_t)k;
            if (sum < target)
                lo = mid + 1;
            else {
                if (mid == 0) break;
                hi = mid - 1;
            }
        }
    }

    /* no suitable k found */
    return (seq_t)0;
}
