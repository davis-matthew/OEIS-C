/*
 * A197227  Primes that are not the sum of at least two consecutive primes.
 * https://oeis.org/A197227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime that is NOT expressible as the sum of at
 *            least * two consecutive primes. The implementation builds a list
 *            of primes up to a * limit, marks those primes that can be
 *            written as a sum of >=2 consecutive * primes, and returns the
 *            nth unmarked prime
 * Range:     u8 n <= 30, u16 n <= 3562, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Utility: grow a dynamic array of 32‑bit integers. */
static void grow_uint32(uint32_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t newcap = (*cap == 0) ? 64 : *cap;
        while (newcap < need) newcap <<= 1;
        uint32_t *newptr = (uint32_t *)realloc(*arr, newcap * sizeof(uint32_t));
        *arr = newptr;
        *cap = newcap;
    }
}

/* Sieve of Eratosthenes up to LIMIT (inclusive).  It returns the list of
   primes (uint32_t) and a bitmap is_prime[0..LIMIT] indicating primality. */
static void sieve_primes_up_to(idx_t LIMIT,
                               uint32_t **primes_out,
                               size_t *prime_cnt_out,
                               uint8_t **is_prime_out)
{
    size_t lim = (size_t)LIMIT + 1;
    uint8_t *is_prime = (uint8_t *)malloc(lim);
    if (!is_prime) abort();
    memset(is_prime, 1, lim);
    is_prime[0] = is_prime[1] = 0;

    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    uint32_t *primes = NULL;
    size_t pcnt = 0, pcap = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            grow_uint32(&primes, &pcap, pcnt + 1);
            primes[pcnt++] = (uint32_t)i;
        }
    }

    *primes_out = primes;
    *prime_cnt_out = pcnt;
    *is_prime_out = is_prime;
}

/* Compute the primes that are NOT sums of >=2 consecutive primes up to LIMIT. */
static void compute_special_primes(idx_t LIMIT,
                                   uint32_t **spec_out,
                                   size_t *spec_cnt_out)
{
    uint32_t *primes = NULL;
    size_t prime_cnt = 0;
    uint8_t *is_prime = NULL;
    sieve_primes_up_to(LIMIT, &primes, &prime_cnt, &is_prime);

    /* Mark primes that ARE sums of at least two consecutive primes. */
    uint8_t *is_sum = (uint8_t *)calloc((size_t)LIMIT + 1, 1);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t sum = 0;
        for (size_t j = i; j < prime_cnt; ++j) {
            sum += primes[j];
            if (sum > LIMIT) break;
            if (j > i && is_prime[sum]) is_sum[sum] = 1;
        }
    }

    /* Collect unmarked primes. */
    uint32_t *special = NULL;
    size_t spec_cnt = 0, spec_cap = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if (!is_sum[p]) {
            grow_uint32(&special, &spec_cap, spec_cnt + 1);
            special[spec_cnt++] = p;
        }
    }

    /* Free temporary buffers. */
    free(primes);
    free(is_prime);
    free(is_sum);

    *spec_out = special;
    *spec_cnt_out = spec_cnt;
}

/* Static state for memoization. */
static bool initialized = false;
static idx_t current_limit = 300000;       /* initial bound */
static uint32_t *special_primes = NULL;    /* array of special primes (uint32_t) */
static size_t special_count = 0;

/* Recompute special primes for a new limit. */
static void recompute_special(idx_t new_limit)
{
    if (special_primes) free(special_primes);
    compute_special_primes(new_limit, &special_primes, &special_count);
    current_limit = new_limit;
}

/* The required public function. */
seq_t a(idx_t n)
{
    if (!initialized) {
        recompute_special(current_limit);
        initialized = true;
    }

    /* Enlarge limit if we ran out of terms. */
    while (n > special_count) {
        recompute_special(current_limit << 1);      /* double the limit */
    }

    /* Convert 1‑based n to 0‑based index. */
    size_t idx = (size_t)(n - 1);
    return (seq_t)special_primes[idx];
}
