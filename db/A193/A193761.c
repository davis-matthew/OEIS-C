/*
 * A193761  0.25-Ramanujan primes R_{0.25,n}: a(n) is the smallest number such
 *          that for any x >= a(n), we have pi(x) - pi(0.25x) >= n, where
 *          pi(x) is the number of primes <= x.
 * https://oeis.org/A193761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest x such that pi(x) – pi(x/4) ≥ n (pi is the prime
 *            counting function)
 * Range:     u8 n <= 36, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/* Choose a sieve bound that comfortably covers all test n values.
   The bound can be increased if a larger range is required. */
static const idx_t MAX_LIMIT = 6000000ULL;            /* 6 000 000 */

/* Static storage for the prime counts and the suffix‑minimum array. */
static uint32_t *pi_arr     = NULL;      /* pi[i] = number of primes ≤ i   */
static uint32_t *minf_arr   = NULL;      /* suffix minimum of f(x) = pi(x)–pi(x/4) */

/* One‑time initialization: build the sieve, pi array and minf_arr. */
static void init_sieve(void)
{
    if (pi_arr != NULL)           /* already initialised */
        return;

    /* Allocate memory (C++ requires a cast). */
    pi_arr   = (uint32_t *)malloc((MAX_LIMIT + 1) * sizeof(uint32_t));
    minf_arr = (uint32_t *)malloc((MAX_LIMIT + 1) * sizeof(uint32_t));
    bool *is_prime = (bool *)malloc((MAX_LIMIT + 1) * sizeof(bool));

    if (!pi_arr || !minf_arr || !is_prime) {
        /* In an evaluation environment we terminate immediately
           if memory cannot be allocated. */
        exit(EXIT_FAILURE);
    }

    /* Mark all indices as potentially prime. */
    for (idx_t i = 0; i <= MAX_LIMIT; ++i)
        is_prime[i] = true;
    if (MAX_LIMIT >= 0) is_prime[0] = false;
    if (MAX_LIMIT >= 1) is_prime[1] = false;

    /* Eratosthenes sieve. */
    for (idx_t i = 2; i * i <= MAX_LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= MAX_LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    /* Build the prime‑count array. */
    uint32_t count = 0;
    for (idx_t i = 0; i <= MAX_LIMIT; ++i) {
        if (is_prime[i]) ++count;
        pi_arr[i] = count;
    }

    /* Compute f(x) = pi(x) – pi(x/4) for every x. */
    for (idx_t i = 0; i <= MAX_LIMIT; ++i)
        minf_arr[i] = pi_arr[i] - pi_arr[i / 4];

    /* Convert to a suffix‑minimum array:
       minf_arr[i] = min_{y ≥ i} f(y). */
    uint32_t cur_min = UINT32_MAX;
    for (idx_t i = MAX_LIMIT + 1; i > 0; --i) {
        idx_t idx = i - 1;
        if (minf_arr[idx] < cur_min)
            cur_min = minf_arr[idx];
        minf_arr[idx] = cur_min;
    }

    /* Free temporary prime flag array. */
    free(is_prime);
}

/* ----------------------------------------------------------------------
   Required public function.                                           
   ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)   /* out of range – not used by OEIS */
        return (seq_t)0;

    init_sieve();          /* Ensure that the sieve data are ready */

    /* Binary search for the smallest x such that minf_arr[x] ≥ n. */
    idx_t lo = 1;
    idx_t hi = MAX_LIMIT;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (minf_arr[mid] >= (uint32_t)n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return (seq_t)lo;
}
