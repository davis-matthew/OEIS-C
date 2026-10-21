/*
 * A095962  If p(x) is the x-th prime, then the n-th set of 3 consecutive sexy
 *          prime pairs starts at p(a(n)).
 * https://oeis.org/A095962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the index in the prime sequence of the first prime of the
 *            n‑th block consisting of 3 consecutive sexy prime pairs. Here
 *            the block is defined by the primes
 *            p(i),p(i+1),p(i+2),p(i+3),p(i+4),p(i+5) where each consecutive
 *            pair has difference 6. This implementation pre‑computes all
 *            primes up to...
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static state --------------------------------------------------------- */
static bool     initialized   = false;          /* one‑time init flag       */
static size_t   prime_limit   = 10000000;       /* sieve upper bound        */

static uint8_t *sieve        = NULL;           /* simple bool array        */
static uint64_t *prime_nums  = NULL;           /* list of primes           */
static size_t   prime_count  = 0;              /* number of primes         */

static idx_t   *start_indices= NULL;           /* block start indices      */
static size_t   start_count  = 0;              /* number of blocks         */

/* Dynamic array helpers ---------------------------------------------- */
static void push_start(idx_t val)
{
    static size_t cap = 0;
    if (start_count == cap) {
        size_t newcap = (cap == 0) ? 64 : cap * 2;
        idx_t *tmp = (idx_t *)realloc(start_indices,
                                      newcap * sizeof(idx_t));
        if (!tmp) exit(EXIT_FAILURE);          /* allocation failure */
        start_indices = tmp;
        cap = newcap;
    }
    start_indices[start_count++] = val;
}

/* Sieve of Eratosthenes ------------------------------------------------ */
static void build_primes(void)
{
    sieve = (uint8_t *)calloc(prime_limit + 1, sizeof(uint8_t));
    if (!sieve) exit(EXIT_FAILURE);

    /* Mark composites */
    for (uint64_t i = 2; i * i <= prime_limit; ++i) {
        if (!sieve[i]) {
            for (uint64_t j = i * i; j <= prime_limit; j += i)
                sieve[j] = 1;
        }
    }

    /* Count primes */
    for (uint64_t i = 2; i <= prime_limit; ++i)
        if (!sieve[i]) ++prime_count;

    /* Allocate array of primes */
    prime_nums = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
    if (!prime_nums) exit(EXIT_FAILURE);

    /* Fill primes array */
    size_t idx = 0;
    for (uint64_t i = 2; i <= prime_limit; ++i)
        if (!sieve[i]) prime_nums[idx++] = i;

    /* Free sieve – not needed afterwards */
    free(sieve);
    sieve = NULL;
}

/* Find all starting indices of blocks of 3 consecutive sexy prime pairs */
static void find_start_indices(void)
{
    for (size_t i = 0; i + 5 < prime_count; ++i) {
        if (prime_nums[i + 1] - prime_nums[i]     == 6 &&
            prime_nums[i + 3] - prime_nums[i + 2] == 6 &&
            prime_nums[i + 5] - prime_nums[i + 4] == 6) {
            /* i is zero‑based, but required index is 1‑based   */
            push_start((idx_t)(i + 1));
        }
    }
}

/* One‑time initialization --------------------------------------------- */
static void ensure_initialized(void)
{
    if (initialized) return;
    build_primes();
    find_start_indices();
    initialized = true;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* offset is 1 */
    ensure_initialized();
    if ((n - 1) >= start_count) return 0;    /* out of range */
    return (seq_t)start_indices[n - 1];
}
