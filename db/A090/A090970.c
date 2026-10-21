/*
 * A090970  Number of primes strictly between T(n) and T(n+1), where T(n) =
 *          n-th triangular number.
 * https://oeis.org/A090970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1010, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_u32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = #{ primes p | T(n) < p < T(n+1) } ,
 * where T(n) = n*(n+1)/2  (triangular numbers)
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/isqrt_u32.h"

/* ---------- dynamic sieve structures ---------- */
static size_t sieve_limit = 0;          /* current sieved upper bound   */
static uint32_t *prime_list = NULL;    /* array of primes up to limit  */
static size_t prime_count = 0;         /* number of stored primes      */

/* ---------- extend the sieve up to at least 'needed' ---------- */
static void ensure_sieve(size_t needed) {
    if (sieve_limit >= needed) return;          /* sufficient already */

    size_t new_limit = needed;
    /* allocate temporary flags array (1 byte per number) */
    uint8_t *is_prime = (uint8_t *)malloc(new_limit + 1);
    if (!is_prime) return;  /* out of memory – caller must not rely on it */

    /* initialize flags */
    for (size_t i = 0; i <= new_limit; ++i) is_prime[i] = 1;
    if (new_limit >= 0) is_prime[0] = 0;
    if (new_limit >= 1) is_prime[1] = 0;

    /* classic sieve of Eratosthenes */
    size_t limit_sqrt = isqrt_u32(new_limit);
    for (size_t i = 2; i <= limit_sqrt; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= new_limit; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    /* count primes and build prime_list */
    size_t cnt = 0;
    for (size_t i = 2; i <= new_limit; ++i) if (is_prime[i]) ++cnt;

    uint32_t *new_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!new_list) { free(is_prime); return; }

    size_t idx = 0;
    for (size_t i = 2; i <= new_limit; ++i) {
        if (is_prime[i]) new_list[idx++] = (uint32_t)i;
    }

    /* replace old data with new data */
    free(is_prime);
    free(prime_list);
    prime_list = new_list;
    prime_count = cnt;
    sieve_limit = new_limit;
}

/* ---------- prime counting: number of primes ≤ x ---------- */
static size_t count_primes_leq(idx_t x) {
    if (prime_count == 0) return 0;
    size_t lo = 0, hi = prime_count;          /* hi is exclusive */
    uint32_t ux = (uint32_t)x;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= ux) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;         /* number of primes <= x */
}

/* ---------- the requested sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;    /* below offset – defined as 0 */

    /* compute triangular numbers in seq_t */
    seq_t Tn  = ((seq_t)n * ((seq_t)n + 1)) / 2;
    seq_t Tn1 = (((seq_t)n + 1) * ((seq_t)n + 2)) / 2;

    /* interval (Tn, Tn1) → [Tn+1, Tn1-1] */
    idx_t low  = (idx_t)(Tn + 1);
    idx_t high = (idx_t)(Tn1 - 1);

    if (low > high) return 0;  /* empty interval */

    /* ensure primes up to 'high' are sieved */
    ensure_sieve(high);

    size_t primes_upto_high  = count_primes_leq(high);
    size_t primes_upto_low_m1 = count_primes_leq(low - 1);

    seq_t result = (seq_t)(primes_upto_high - primes_upto_low_m1);
    return result;
}
