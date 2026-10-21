/*
 * A093874  Least k such that there are at least n primes between k and k^2.
 * https://oeis.org/A093874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  less, nonn
 * Formula:   Least k such that there are at least n primes strictly between k
 *            and k^2
 * Range:     u8 n <= 39, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#define SIEVE_LIMIT 2000000U

static uint32_t *pi_arr = NULL;       /* prefix prime counts */
static idx_t     pi_limit = 0;        /* maximum index of pi_arr */
static idx_t     max_k   = 0;        /* largest k with k^2 <= pi_limit */
static bool      initialized = false;

/* Allocate a sieve and compute pi_arr */
static void init_sieve(void) {
    if (initialized) return;

    pi_limit = SIEVE_LIMIT;
    pi_arr = (uint32_t *)malloc((pi_limit + 1) * sizeof(uint32_t));
    if (!pi_arr) return;                /* allocation failed */

    uint8_t *is_prime = (uint8_t *)malloc((pi_limit + 1) * sizeof(uint8_t));
    if (!is_prime) {
        free(pi_arr);
        pi_arr = NULL;
        return;
    }

    /* Simple Sieve of Eratosthenes */
    for (idx_t i = 0; i <= pi_limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    idx_t sqrt_limit = 0;
    while ((sqrt_limit + 1) * (sqrt_limit + 1) <= pi_limit) ++sqrt_limit;

    for (idx_t p = 2; p <= sqrt_limit; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= pi_limit; m += p) {
                is_prime[m] = 0;
            }
        }
    }

    /* Build prefix prime counts */
    pi_arr[0] = 0;
    for (idx_t i = 1; i <= pi_limit; ++i) {
        pi_arr[i] = pi_arr[i - 1] + (uint32_t)is_prime[i];
    }

    free(is_prime);

    /* Compute maximum k with k^2 <= pi_limit */
    max_k = 0;
    while ((max_k + 1) * (max_k + 1) <= pi_limit) ++max_k;

    initialized = true;
}

/* Count primes strictly between k and k^2 */
static inline seq_t primes_between(idx_t k) {
    idx_t a = k * k;
    if (a > 0) --a;                       /* a = k^2 - 1 */
    if (a > pi_limit) return 0;           /* safety */
    return (seq_t)(pi_arr[a] - pi_arr[k]); /* pi(k^2 - 1) - pi(k) */
}

/* OEIS sequence A093874 */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;           /* undefined for n < 1 */

    init_sieve();
    if (!pi_arr) return (seq_t)0;         /* initialization failed */

    idx_t low  = 2;
    idx_t high = max_k;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t cnt = primes_between(mid);
        if (cnt >= (seq_t)n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return (seq_t)low;
}
