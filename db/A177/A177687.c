/*
 * A177687  Number of distinct transpositions of digits (zeros and units) in
 *          n-th prime written in base 2.
 * https://oeis.org/A177687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = binomial(len(p), popcount(p)) where p is the n‑th prime,
 *            len(p) is the length of its binary representation (A023416 +
 *            A000120), and popcount(p) is its number of ones (A000120)
 * Range:     u8 n <= 172, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation works in both C11 and C++17; seq_t and idx_t
   are defined by the build system. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Types are supplied by the build environment. */
typedef idx_t  idx_t;   /* 64‑bit unsigned integer for indices           */
typedef seq_t  seq_t;   /* unsigned integer type for sequence values  */

/* ---------- small helpers ---------- */

/* floor(log2(v)) for v > 0; zero is not used for primes */
static inline idx_t floor_log2_uint64(idx_t v)
{
    return (idx_t)(63 - __builtin_clzll(v));
}

/* Compute C(n,k) with 0 <= k <= n, using only integer arithmetic. */
static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;          /* symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = (res * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return res;
}

/* ---------- prime generation (sieving) ---------- */

static bool primes_built = false;
static uint64_t *primes = NULL;
static size_t   primes_cnt = 0;
static idx_t    sieve_limit = 2000000;          /* initial upper bound */

/* Build a sieve up to 'limit' and store the primes in the global array. */
static void build_sieve(idx_t limit)
{
    bool *sieve = (bool *)calloc((size_t)limit + 1U, 1U);
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                sieve[j] = true;
            }
        }
    }

    /* Count primes and allocate array. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) ++cnt;
    }

    uint64_t *arr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) arr[k++] = (uint64_t)i;
    }

    free(sieve);

    /* Replace any existing array. */
    if (primes) free(primes);
    primes = arr;
    primes_cnt = cnt;
    sieve_limit = limit;
}

/* Ensure that the global prime array contains at least 'n' primes. */
static void ensure_primes(idx_t n)
{
    if (primes_built && n <= primes_cnt) return;

    /* Rebuild the sieve, doubling its limit until we have enough primes. */
    idx_t lim = sieve_limit;
    while (true) {
        build_sieve(lim);
        if (primes_cnt >= n) break;
        lim *= 2;
    }
    primes_built = true;
}

/* ---------- sequence function ---------- */

/* a(n): number of distinct transpositions of the binary digits of the n‑th prime. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 1 – guard clause */

    ensure_primes(n);

    uint64_t p = primes[n - 1];          /* 0‑based array index */

    uint64_t ones = __builtin_popcountll(p);
    uint64_t len  = floor_log2_uint64(p) + 1U;

    return binomial(len, ones);
}
