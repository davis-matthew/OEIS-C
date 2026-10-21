/*
 * A097935  Number of primes that are not less than prime(n)-n and not greater
 *          than prime(n)+n.
 * https://oeis.org/A097935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #primes p: prime(n)-n <= p <= prime(n)+n
 * Range:     u8 n <= 1139, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Static storage for the list of primes */
static uint64_t *prime_list = NULL;      /* array of primes (ascending)          */
static size_t   prime_cnt   = 0;          /* number of primes currently stored    */
static size_t   prime_cap   = 0;          /* current capacity of the array        */

/* Expand the prime array to accommodate at least 'need' elements */
static void ensure_capacity(size_t need)
{
    if (prime_cap >= need) return;
    size_t new_cap = prime_cap ? prime_cap : 16;
    while (new_cap < need) new_cap *= 2;
    uint64_t *new_arr = (uint64_t*)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!new_arr) {
        /* Allocation failure – abort – this function is not expected to be called
           in an environment where such low memory is available. */
        abort();
    }
    prime_list = new_arr;
    prime_cap  = new_cap;
}

/* Test whether a 64‑bit number is prime using trial division by known primes.
   Only primes up to sqrt(n) are used. */
static int is_prime_candidate(uint64_t n)
{
    if (n < 2) return 0;
    for (size_t i = 0; i < prime_cnt && prime_list[i]*prime_list[i] <= n; ++i) {
        if (n % prime_list[i] == 0) return 0;
    }
    return 1;
}

/* Generate primes until we have at least 'needed' primes. */
static void ensure_nth_prime(idx_t needed)
{
    while (prime_cnt < (size_t)needed) {
        uint64_t candidate;
        if (prime_cnt == 0) {
            candidate = 2;
            let:
            candidate = 2;
            if (is_prime_candidate(candidate)) {
                ensure_capacity(prime_cnt + 1);
                prime_list[prime_cnt++] = candidate;
            }
            continue;
        }
        /* Next odd candidate after the last prime */
        candidate = prime_list[prime_cnt - 1] + 1;
        if (candidate % 2 == 0) ++candidate;
        while (!is_prime_candidate(candidate)) {
            candidate += 2;
        }
        ensure_capacity(prime_cnt + 1);
        prime_list[prime_cnt++] = candidate;
    }
}

/* Generate primes up to and including the first prime strictly greater than 'limit'.
   After this call, prime_list[prime_cnt-1] > limit. */
static void ensure_primes_upto_plus1(uint64_t limit)
{
    /* If we already have a prime > limit, nothing to do. */
    if (prime_cnt > 0 && prime_list[prime_cnt - 1] > limit) return;
    if (prime_cnt == 0) {
        /* Start from the first prime (2). */
        ensure_nth_prime(1);
    }
    uint64_t candidate = prime_list[prime_cnt - 1] + 1;
    if (candidate % 2 == 0) ++candidate;
    while (prime_list[prime_cnt - 1] <= limit || prime_list[prime_cnt - 1] <= limit) {
        while (!is_prime_candidate(candidate)) candidate += 2;
        ensure_capacity(prime_cnt + 1);
        prime_list[prime_cnt++] = candidate;
        if (prime_list[prime_cnt - 1] > limit) break;
        candidate += 2;
    }
}

/* Count of primes <= x (π(x)), using binary search on the sorted prime list. */
static idx_t pi_u64(uint64_t x)
{
    if (prime_cnt == 0 || x < 2) return 0;
    size_t l = 0, r = prime_cnt;
    while (l < r) {
        size_t m = (l + r) >> 1;
        if (prime_list[m] <= x)
            l = m + 1;
        else
            r = m;
    }
    return (idx_t)l;   /* number of primes <= x */
}

/* Main sequence function. */
extern seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n<1, return 0 */

    /* Ensure we have the n-th prime. */
    ensure_nth_prime(n);

    uint64_t p_n = prime_list[n - 1];   /* prime(n) */

    /* Bounds of the interval. */
    uint64_t lower = (p_n > (uint64_t)n) ? p_n - (uint64_t)n : 1;
    uint64_t upper = p_n + (uint64_t)n;

    /* Ensure we have primes up to at least 'upper' so that pi(upper) works. */
    ensure_primes_upto_plus1(upper);

    /* Compute π(upper) and π(lower-1). */
    idx_t pi_upper      = pi_u64(upper);
    idx_t pi_lower_minus = (lower > 1) ? pi_u64(lower - 1) : 0;

    /* Result fits in seq_t because the count is <= upper. */
    return (seq_t)(pi_upper - pi_lower_minus);
}
