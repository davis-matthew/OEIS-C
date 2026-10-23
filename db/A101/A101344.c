/*
 * A101344  Number of primes between prime(n) and 3prime(n).
 * https://oeis.org/A101344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   estimate_prime_upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = #{p prime | prime(n) < p ≤ 3·prime(n)} */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/estimate_prime_upper_bound.h"

/* -------------------------------------------------------------------- */
/* internal state                                                      */
/* -------------------------------------------------------------------- */
static struct {
    uint32_t *primes;      /* array of primes in ascending order            */
    uint32_t prime_count;  /* number of primes stored                       */
    uint64_t limit;        /* maximum value for which we have a sieve      */
} sieve_state = { NULL, 0, 0 };

/* -------------------------------------------------------------------- */
/* helper: estimate upper bound for n‑th prime                          */

/* -------------------------------------------------------------------- */
/* helper: build sieve and prime list up to 'limit'                     */
/* -------------------------------------------------------------------- */
static void rebuild_sieve(uint64_t limit)
{
    uint8_t *is_prime = (uint8_t *)malloc(limit + 1);
    if (!is_prime) exit(1);
    for (uint64_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    uint64_t sq = (uint64_t)sqrt((double)limit);
    for (uint64_t p = 2; p <= sq; ++p) {
        if (is_prime[p]) {
            for (uint64_t j = p * p; j <= limit; j += p) is_prime[j] = 0;
        }
    }
    /* count primes first */
    uint32_t count = 0;
    for (uint64_t i = 2; i <= limit; ++i) if (is_prime[i]) ++count;
    uint32_t *prime_arr = (uint32_t *)malloc(count * sizeof(uint32_t));
    if (!prime_arr) { free(is_prime); exit(1); }

    uint32_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) prime_arr[idx++] = (uint32_t)i;

    free(is_prime);
    free(sieve_state.primes);

    sieve_state.primes     = prime_arr;
    sieve_state.prime_count = count;
    sieve_state.limit      = limit;
}

/* -------------------------------------------------------------------- */
/* helper: ensure sieve covers at least n‑th prime and 3·p_n             */
/* -------------------------------------------------------------------- */
static void ensure_sieve_for_n(idx_t n)
{
    if (!sieve_state.primes) {
        uint64_t bound = estimate_prime_upper_bound(n);
        rebuild_sieve(3 * bound);
        return;
    }
    if (n > sieve_state.prime_count ||
        (uint64_t)3 * sieve_state.primes[n - 1] > sieve_state.limit) {
        uint64_t bound = estimate_prime_upper_bound(n);
        rebuild_sieve(3 * bound);
    }
}

/* -------------------------------------------------------------------- */
/* helper: count primes ≤ x using binary search on prime array         */
/* -------------------------------------------------------------------- */
static inline uint64_t count_primes_leq(uint64_t x)
{
    idx_t left = 0, right = sieve_state.prime_count;
    while (left < right) {
        idx_t mid = left + ((right - left) >> 1);
        if ((uint64_t)sieve_state.primes[mid] <= x)
            left = mid + 1;
        else
            right = mid;
    }
    return (uint64_t)left;
}

/* -------------------------------------------------------------------- */
/* public function                                                     */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1, return 0 as safe value */

    ensure_sieve_for_n(n);

    uint32_t pn   = sieve_state.primes[n - 1];
    uint64_t lower = (uint64_t)pn;
    uint64_t upper = 3 * lower;

    uint64_t cnt = count_primes_leq(upper) - count_primes_leq(lower);
    return (seq_t)cnt;
}
