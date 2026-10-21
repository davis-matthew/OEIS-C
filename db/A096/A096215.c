/*
 * A096215  Greatest primes not greater than the sum of two succeeding primes.
 * https://oeis.org/A096215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007917(A001043(n)) A001043(n) = p_n + p_{n+1} (sum of
 *            the n-th and (n+1)-th primes) A007917(k) = greatest prime ≤ k
 *            The implementation builds a digit‑independent prime list on
 *            demand and returns the largest prime not exceeding the sum of
 *            two successive primes
 * Range:     u8 n <= 30, u16 n <= 3512, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static size_t prime_count = 0;          /* number of primes stored          */
static size_t prime_cap   = 0;          /* allocated capacity of prime_arr   */
static uint64_t *prime_arr = NULL;     /* array of primes (uint64_t)       */

/* Ensure prime_arr has room for one more prime; grow capacity as needed. */
static void grow_prime_array(void)
{
    if (prime_cap == 0) {
        prime_cap = 16;
        prime_arr = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
    } else if (prime_count == prime_cap) {
        prime_cap *= 2;
        prime_arr = (uint64_t*)realloc(prime_arr, prime_cap * sizeof(uint64_t));
    }
}

/* Test whether x is prime using the primes already stored (up to sqrt(x)). */
static bool is_prime_candidate(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_arr[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Generate the next prime greater than the last one stored. */
static void generate_next_prime(void)
{
    uint64_t candidate = (prime_count == 0) ? 2 : prime_arr[prime_count - 1] + 1;
    while (true) {
        if (is_prime_candidate(candidate)) {
            grow_prime_array();
            prime_arr[prime_count++] = candidate;
            return;
        }
        ++candidate;
    }
}

/* Ensure that at least 'cnt' primes are stored. */
static void ensure_primes_count(size_t cnt)
{
    while (prime_count < cnt) {
        generate_next_prime();
    }
}

/* Ensure that primes are available up to at least 'limit'. */
static void ensure_primes_up_to(uint64_t limit)
{
    if (limit < 2) return;
    while (prime_count == 0 || prime_arr[prime_count - 1] < limit) {
        generate_next_prime();
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* offset is 1; undefined for 0 */

    /* 1. Need primes p_n and p_{n+1}. */
    size_t need_cnt = (size_t)n + 1;         /* because n is 1‑based */
    ensure_primes_count(need_cnt);

    uint64_t p_n   = prime_arr[ (size_t)n - 1 ];
    uint64_t p_next = prime_arr[ (size_t)n ];

    /* 2. Compute the sum s = p_n + p_{n+1}. */
    uint64_t s = p_n + p_next;

    /* 3. Ensure primes are available up to s. */
    ensure_primes_up_to(s);

    /* 4. Find the largest prime ≤ s (binary search). */
    size_t lo = 0;
    size_t hi = prime_count;  /* exclusive */
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (prime_arr[mid] <= s) lo = mid + 1;
        else                      hi = mid;
    }
    size_t idx = lo - 1;  /* guaranteed at least 0 because s ≥ 2 */

    return (seq_t)prime_arr[idx];
}
