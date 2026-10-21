/*
 * A078731  Sum of primes between successive pairs of twin primes.
 * https://oeis.org/A078731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of primes strictly between the nth and (n+1)th
 *            twin‑prime pairs. a(1) = 0 (between (3,5) and (5,7)), etc
 * Range:     u8 n <= 7, u16 n <= 130, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#include "../utils/alloc_seq_array.h"

static const idx_t LIMIT = 2000000ULL;          /* sufficient for n <= 10000 */

static int built = 0;                           /* initialization flag */

static unsigned char *is_prime = NULL;          /* sieve bitmap */
static seq_t *prime_prefix = NULL;              /* prefix sums of primes */
static idx_t *twin_low = NULL;                 /* low member of each twin pair */
static size_t twin_cnt = 0;                     /* number of twin pairs found */

/* Build sieve, prime prefix sums and twin‑prime list */
static void build(void)
{
    if (built) return;

    /* allocate sieve bitmap */
    is_prime = (unsigned char*)calloc((size_t)(LIMIT + 1), sizeof(unsigned char));
    if (!is_prime) return;

    /* mark all numbers >=2 as prime initially */
    for (idx_t i = 2; i <= LIMIT; ++i) is_prime[i] = 1;

    /* classic Eratosthenes sieve */
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* allocate prefix sum array (seq_t type) */
    prime_prefix = alloc_seq_array((size_t)(LIMIT + 1));
    if (!prime_prefix) {
        free(is_prime);
        return;
    }

    /* compute prefix sums of primes */
    prime_prefix[0] = 0;
    for (idx_t i = 1; i <= LIMIT; ++i) {
        if (is_prime[i])
            prime_prefix[i] = prime_prefix[i - 1] + (seq_t)i;
        else
            prime_prefix[i] = prime_prefix[i - 1];
    }

    /* estimate capacity for twin pair list */
    size_t cap = LIMIT / 10 + 10;
    twin_low = (idx_t*)malloc(cap * sizeof(idx_t));
    if (!twin_low) {
        free(is_prime);
        free(prime_prefix);
        return;
    }

    /* collect twin‑prime pairs (low member only) */
    twin_cnt = 0;
    for (idx_t i = 2; i + 2 <= LIMIT; ++i) {
        if (is_prime[i] && is_prime[i + 2]) {
            if (twin_cnt == cap) {
                cap *= 2;
                idx_t *tmp = (idx_t*)realloc(twin_low, cap * sizeof(idx_t));
                if (!tmp) { free(is_prime); free(prime_prefix); free(twin_low); return; }
                twin_low = tmp;
            }
            twin_low[twin_cnt++] = i;
        }
    }

    built = 1;
}

/* Return a(n) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (!built) build();
    if (!built) return 0;               /* failed to initialise */

    /* need at least n+1 twin pairs (0‑based indices n-1 and n) */
    if ((size_t)n >= twin_cnt) return 0;

    idx_t low1 = twin_low[n - 1];
    idx_t high1 = low1 + 2;
    idx_t low2 = twin_low[n];

    if (low2 <= high1 + 1) return 0;    /* no primes strictly between pairs */

    /* sum of primes in (high1, low2) = prefix[low2-1] - prefix[high1] */
    return prime_prefix[low2 - 1] - prime_prefix[high1];
}
