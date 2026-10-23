/*
 * A037040  Number of odd nonprimes < (2n+1)^2.
 * https://oeis.org/A037040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n*(n+1) - pi(4*n*(n+1)) + (4*n*(n+1) > 3 ? 1 : 0) where
 *            pi(x) is the number of primes ≤ x
 * Range:     u8 n <= 13, u16 n <= 199, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static int _a_initialized = 0;

/* maximum n considered by the problem specification */
#define MAX_N 10000U
/* maximum limit = (2*MAX_N+1)^2 = 4*MAX_N*(MAX_N+1) */
#define MAX_LIMIT ((idx_t)4U * (idx_t)MAX_N * ((idx_t)MAX_N + 1U))

/* bit‑set for odd numbers: bit k corresponds to number 2*k+1 */
static uint8_t *sieve_bits = NULL;
static size_t   sieve_bytes = 0;

/* array of primes found by the sieve */
static uint32_t *primes   = NULL;
static size_t    primes_cap = 0;
static size_t    primes_len = 0;

/* macro helpers for bit operations on sieve_bits */
#define BIT_SET(arr, k)   ((arr)[(k)>>3] |=  (1U << ((k)&7)))
#define BIT_GET(arr, k)   ((arr)[(k)>>3] &   (1U << ((k)&7)))

/* extend the primes array when needed */
static void _primes_grow(void)
{
    size_t new_cap = primes_cap ? primes_cap << 1U : (1U << 20U);
    uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
    if (!tmp) abort(); /* memory allocation failure */
    primes = tmp;
    primes_cap = new_cap;
}

/* initialize the global sieve and prime tables */
static void _initialize(void)
{
    if (_a_initialized) return;
    _a_initialized = 1;

    /* size of sieve: store odd numbers up to MAX_LIMIT, inclusive */
    size_t odd_bits = (MAX_LIMIT >> 1U) + 1U;   /* bits needed for (MAX_LIMIT+1)/2 odd numbers */
    sieve_bytes  = (odd_bits + 7U) >> 3U;       /* bytes needed for the bitset */
    sieve_bits   = (uint8_t *)calloc(sieve_bytes, sizeof(uint8_t));
    if (!sieve_bits) abort();

    /* Sieve of Eratosthenes on odds */
    for (idx_t i = 3U; i * i <= MAX_LIMIT; i += 2U) {
        idx_t idx = i >> 1U;
        if (!BIT_GET(sieve_bits, idx)) {
            /* i is prime */
            for (idx_t j = i * i; j <= MAX_LIMIT; j += i << 1U) {
                BIT_SET(sieve_bits, j >> 1U);
            }
        }
    }

    /* collect primes into array */
    primes_cap = 1U << 20U; /* initial capacity ~1M */
    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) abort();

    /* prime 2 */
    primes[0] = 2U;
    primes_len = 1U;

    /* odd primes */
    for (idx_t i = 3U; i <= MAX_LIMIT; i += 2U) {
        if (!BIT_GET(sieve_bits, i >> 1U)) {
            if (primes_len == primes_cap) _primes_grow();
            primes[primes_len++] = (uint32_t)i;
        }
    }
}

/* count of primes ≤ x using binary search on primes array */
static inline size_t _count_primes_leq(idx_t x)
{
    if ((idx_t)2 == 0) return 0; /* never reached */
    if (x < 2U) return 0U;
    size_t lo = 0U, hi = primes_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1U;
        if ((idx_t)primes[mid] <= x) lo = mid + 1U;
        else hi = mid;
    }
    return lo;
}

/* OEIS sequence A037040 */
seq_t a(idx_t n)
{
    _initialize();

    /* limit = (2*n+1)^2 */
    idx_t h = (idx_t)((seq_t)n * 2U + 1U);
    idx_t limit = h * h;                 /* (2n+1)^2 */
    idx_t limit_minus1 = limit - 1U;     /* m-1 */

    /* number of odd numbers < limit = (limit-1)/2 = 2*n*(n+1) */
    seq_t odd = 2U * (seq_t)n * ((seq_t)n + 1U);

    /* prime counting function */
    seq_t pi = (seq_t)_count_primes_leq(limit_minus1);

    /* subtract 1 only if 2 ≤ limit-1 (i.e., limit > 3) */
    seq_t add_one = (limit > 3U) ? 1U : 0U;

    return odd - pi + add_one;
}
