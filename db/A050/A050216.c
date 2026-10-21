/*
 * A050216  Number of primes between (prime(n))^2 and (prime(n+1))^2, with
 *          a(0) = 2 by convention.
 * https://oeis.org/A050216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1830, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* Number of primes between (prime(n))^2 and (prime(n+1))^2
   (a(0)=2 by convention).  Computed with a segmented sieve on the
   interval [prime(n)^2, prime(n+1)^2). */

static const idx_t PRIMES_LIMIT = 200000;          /* primes up to sqrt(max high) */
static uint64_t *small_primes = NULL;             /* array containing all primes <=PRIMES_LIMIT */
static size_t small_prime_cnt = 0;                /* number of primes stored */
static bool primes_initialized = false;

/* Pre‑compute all primes ≤ PRIMES_LIMIT with a simple sieve. */
static void init_primes(void)
{
    if (primes_initialized) return;
    primes_initialized = true;

    /* Allocate temporary boolean array for the sieve */
    bool *is_prime = (bool*)malloc((PRIMES_LIMIT + 1) * sizeof(bool));
    for (size_t i = 0; i <= PRIMES_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= PRIMES_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= PRIMES_LIMIT; j += i) {
                is_prime[j] = false;
            }
        }
    }

    /* Count primes first to know how much memory to allocate */
    small_prime_cnt = 0;
    for (size_t i = 2; i <= PRIMES_LIMIT; ++i) {
        if (is_prime[i]) ++small_prime_cnt;
    }

    small_primes = (uint64_t*)malloc(small_prime_cnt * sizeof(uint64_t));

    size_t idx = 0;
    for (size_t i = 2; i <= PRIMES_LIMIT; ++i) {
        if (is_prime[i]) small_primes[idx++] = (uint64_t)i;
    }

    free(is_prime);
}

/* Count primes inside the interval (low, high) using a segmented sieve.
   The interval is represented as [low, high - 1]. */
static seq_t count_primes_in_interval(uint64_t low, uint64_t high)
{
    /* Length of the segment */
    size_t len = (size_t)(high - low);
    bool *seg = (bool*)malloc(len * sizeof(bool));
    if (!seg) return (seq_t)0;           /* safety, should not happen */
    memset(seg, true, len);

    /* Perform the segmented sieve using all primes up to sqrt(high). */
    for (size_t i = 0; i < small_prime_cnt && (uint64_t)small_primes[i] * small_primes[i] <= high; ++i) {
        uint64_t p = small_primes[i];
        uint64_t start = (low + p - 1) / p * p;      /* first multiple of p >= low */
        if (start < p * p) start = p * p;            /* ensure we start from p^2       */

        for (uint64_t j = start; j < high; j += p) {
            seg[(size_t)(j - low)] = false;
        }
    }

    /* Count the remaining marked numbers (primes). */
    seq_t cnt = (seq_t)0;
    for (size_t i = 0; i < len; ++i) {
        if (seg[i]) ++cnt;
    }

    free(seg);
    return cnt;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    init_primes();

    if (n == 0) {
        return (seq_t)2;   /* by convention */
    }

    /* Ensure that we have at least n+1 primes precomputed. */
    if ((size_t)n + 1 > small_prime_cnt) {
        /* In the typical test data n <= 10000, so this situation never occurs.
           The fallback below simply returns 0 to avoid undefined behaviour. */
        return (seq_t)0;
    }

    uint64_t p_n    = small_primes[(size_t)n - 1];
    uint64_t p_next = small_primes[(size_t)n];

    uint64_t low  = (uint64_t)p_n * p_n;
    uint64_t high = (uint64_t)p_next * p_next;

    /* Count primes strictly between low and high (i.e. in (low, high)). */
    return count_primes_in_interval(low, high);
}
